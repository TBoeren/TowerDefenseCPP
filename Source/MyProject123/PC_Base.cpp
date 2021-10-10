// Fill out your copyright notice in the Description page of Project Settings.
#include "PC_Base.h"

APC_Base::APC_Base()
{
}

// Called when the game starts or when spawned
void APC_Base::BeginPlay()
{
    Super::BeginPlay();

    II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->GetGrids(Grids);
    }
    else
    {
        return;
    }
}

void APC_Base::SetupInputComponent()
{
    Super::SetupInputComponent();
    //Bind the inputs of the mouse to the function to call

    InputComponent->BindAxis(TEXT("MouseMove"), this, &APC_Base::MouseMove);
    InputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &APC_Base::CameraZoomIn);
    InputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &APC_Base::CameraZoomOut);
    InputComponent->BindAction(TEXT("SelectButton"), IE_Pressed, this, &APC_Base::OnSelectButtonDown);
    InputComponent->BindAction(TEXT("CancelButton"), IE_Pressed, this, &APC_Base::OnCancelButtonDown);
}

void APC_Base::MouseMove(float Value)
{
    //When the mouse moves,
    FVector2D MousePosition;

    //Set the mouse position and move the camera if near the edge
    if (GetMousePosition(MousePosition.X, MousePosition.Y) && !LocationSelected)
    {
        II_BasePawn *PawnInterface = Cast<II_BasePawn>(GetPawn());
        if (PawnInterface)
        {
            PawnInterface->EdgeScrollCamera(APC_Base::CalculateEdgeScroll(MousePosition));
        }
    }

    if (!LocationSelected)
    {
        for (AActor *Grid : Grids)
        {
            II_Grid *GridInterface = Cast<II_Grid>(Grid);
            if (GridInterface)
            {
                //Check if you are on the grid
                FHitResult Result;
                if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Result))
                {
                    //Convert the location of the mouse to the grid location
                    int Row;
                    int Column;

                    if (GridInterface->LocationToTile(Result.Location, false, Row, Column))
                    {
                        //If the location is on the grid, highlight the appropriate tile
                        GridInterface->SetSelectedTile(Row, Column);
                    }
                    else
                    {
                        //If it is not on the grid, pass a false value to set the visibility back to hidden
                        GridInterface->SetSelectedTile(-1, -1);
                    }
                }
            }
        }
    }
}

void APC_Base::OnSelectButtonDown()
{
    if (!LocationSelected)
    {
        FVector2D GridLocation;

        FHitResult Result;
        if (GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Result))
        {
            //If a tower is hit, open the tower upgrade menu TODO
            if (Result.GetActor()->IsValidLowLevel() && Result.GetActor()->IsA<ATowerBase>())
            {
                for (AActor *Grid : Grids)
                {
                    //Find the grid that the tower to sell is located on
                    II_Grid *GridInterface = Cast<II_Grid>(Grid);
                    if (GridInterface)
                    {
                        //Convert the location of the mouse to the grid location
                        int Row;
                        int Column;

                        if (GridInterface->LocationToTile(Result.GetActor()->GetActorLocation(), true, Row, Column))
                        {
                            //Save the selected tile and prevent the hover check from continuing
                            CurrentlySelectedGrid = Grid;

                            //Show the tower that is selected on the build menu
                            LocationSelected = true;
                            OnTowerSelected.Broadcast(Result.GetActor());
                        }
                    }
                }
            }
            else
            {
                for (AActor *Grid : Grids)
                {
                    //Check if you are on the grid
                    II_Grid *GridInterface = Cast<II_Grid>(Grid);
                    if (GridInterface)
                    {
                        //Convert the location of the mouse to the grid location
                        int Row;
                        int Column;

                        if (GridInterface->LocationToTile(Result.Location, false, Row, Column))
                        {
                            //Save the selected tile and prevent the hover check from continuing
                            LocationSelected = true;
                            OnTileSelected.Broadcast(FIntPoint(Row, Column));
                            CurrentlySelectedGrid = Grid;
                        }
                    }
                }
            }
        }
    }
}

void APC_Base::OnCancelButtonDown()
{
    //For now, allows the hovered tile to update properly again
    if (LocationSelected)
    {
        LocationSelected = false;
        OnTileUnselected.Broadcast(FIntPoint(-1, -1));
        CurrentlySelectedGrid = nullptr;
    }
}

FVector2D APC_Base::CalculateEdgeScroll(FVector2D MousePosition)
{
    FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    float XValue = ((MousePosition.X / ViewportSize.X) - 0.5);
    float YValue = ((MousePosition.Y / ViewportSize.Y) - 0.5) * -1;

    if (FMath::Abs(XValue) > EdgeScrollTolerance || FMath::Abs(YValue) > EdgeScrollTolerance)
    {
        return FVector2D((YValue * EdgeScrollSpeed), (XValue * EdgeScrollSpeed));
    }

    return FVector2D(0, 0);
}

void APC_Base::CameraZoomIn()
{
    II_BasePawn *PawnInterface = Cast<II_BasePawn>(GetPawn());
    if (PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * -1.0));
    }
}

void APC_Base::CameraZoomOut()
{
    II_BasePawn *PawnInterface = Cast<II_BasePawn>(GetPawn());
    if (PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * 1.0));
    }
}

void APC_Base::PassTowerToConstruct_Implementation(FName TowerRowName)
{
    //Get the proper class from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats *TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //and pass it to the Tower construct function
    II_Grid *GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->ConstructTower(TowerStats->TowerBlueprint);

        II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
        if (GameStateInterface)
        {
            GameStateInterface->SetResources(-TowerStats->TowerCost);
        }
    }
    APC_Base::OnCancelButtonDown();
}

void APC_Base::PassTowerRange_Implementation(FName TowerRowName)
{
    //Get the proper class from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats *TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //and pass its range to the grid
    II_Grid *GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->ConstructRangeDecal(TowerStats->TowerRange);
    }
}

void APC_Base::PassTowerSell_Implementation(FName TowerRowName, AActor *TowerToSell)
{
    //Get the cost from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats *TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //Call the grid and tell it to sell this tower
    II_Grid *GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->SellTower(TowerToSell);

        //Update the resources with the sell value of the tower
        II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
        if (GameStateInterface)
        {
            int SellValue = ((TowerStats->TowerCost / 5) * 3);
            GameStateInterface->SetResources(SellValue);
        }
    }

    //Once it is sold, hide the menu again
    APC_Base::OnCancelButtonDown();
}
