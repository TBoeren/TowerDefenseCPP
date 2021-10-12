// Fill out your copyright notice in the Description page of Project Settings.
#include "PC_Base.h"

APC_Base::APC_Base()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APC_Base::BeginPlay()
{
    Super::BeginPlay();

    //Get the available grids from the game state and set the grids variable
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->GetGrids(Grids);
    }
    else
    {
        return;
    }

    //Create the main widget
    if(!APC_Base::IsInMainMenu())
    {
        UUserWidget* MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass, FName("Main Widget"));
        MainWidget->AddToViewport();
    }
}

void APC_Base::SetupInputComponent()
{
    Super::SetupInputComponent();

    //Bind the inputs of the mouse and keyboard to the function to call
    InputComponent->BindAxis(TEXT("MouseMove"), this, &APC_Base::MouseMove);
    InputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &APC_Base::CameraZoomIn);
    InputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &APC_Base::CameraZoomOut);
    InputComponent->BindAction(TEXT("SelectButton"), IE_Pressed, this, &APC_Base::OnSelectButtonDown);
    InputComponent->BindAction(TEXT("CancelButton"), IE_Pressed, this, &APC_Base::OnCancelButtonDown);
    InputComponent->BindAction(TEXT("PauseGame"), IE_Pressed, this, &APC_Base::UpdatePause).bExecuteWhenPaused = true;
    InputComponent->BindAction(TEXT("UpdateSpeed"), IE_Pressed, this, &APC_Base::UpdateGameSpeed);
    InputComponent->BindAction(TEXT("UpdateSpeed"), IE_Released, this, &APC_Base::UpdateGameSpeed);
}

void APC_Base::MouseMove(float Value)
{
    //When the mouse moves,
    FVector2D MousePosition;

    //Set the mouse position and move the camera if near the edge
    if (GetMousePosition(MousePosition.X, MousePosition.Y) && !LocationSelected)
    {
        II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
        if (PawnInterface)
        {
            PawnInterface->EdgeScrollCamera(APC_Base::CalculateEdgeScroll(MousePosition));
        }
    }

    //If no tile or tower has been selected
    if (!LocationSelected)
    {
        for (AActor* Grid : Grids)
        {
            II_Grid* GridInterface = Cast<II_Grid>(Grid);
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
            //If a tower is hit, open the tower menu
            if (Result.GetActor()->IsValidLowLevel() && Result.GetActor()->IsA<ATowerBase>())
            {
                for (AActor* Grid : Grids)
                {
                    //Find the grid that the tower to sell is located on
                    II_Grid* GridInterface = Cast<II_Grid>(Grid);
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
                for (AActor* Grid : Grids)
                {
                    //Check if you are on the grid
                    II_Grid* GridInterface = Cast<II_Grid>(Grid);
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
    //Allows the hovered tile to update and allow edge scrolling again
    if (LocationSelected)
    {
        LocationSelected = false;
        OnTileUnselected.Broadcast(FIntPoint(-1, -1));
        CurrentlySelectedGrid = nullptr;
    }
}

FVector2D APC_Base::CalculateEdgeScroll(FVector2D MousePosition)
{
    //Get the viewport size
    FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    //Calculate the current range that the mouse located in
    float XValue = ((MousePosition.X / ViewportSize.X) - 0.5);
    float YValue = ((MousePosition.Y / ViewportSize.Y) - 0.5) * -1;

    //Check if the mouse is close enough to the edges to allow scrolling and if it is, return the offset to add to the camera
    if (FMath::Abs(XValue) > EdgeScrollTolerance || FMath::Abs(YValue) > EdgeScrollTolerance)
    {
        return FVector2D((YValue * EdgeScrollSpeed), (XValue * EdgeScrollSpeed));
    }

    return FVector2D(0, 0);
}

void APC_Base::CameraZoomIn()
{
    //Tell the pawn to update the camera boom to zoom in
    II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
    if (PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * -1.0));
    }
}

void APC_Base::CameraZoomOut()
{
    //Tell the pawn to update the camera boom to zoom out
    II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
    if (PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * 1.0));
    }
}

void APC_Base::UpdateGameSpeed()
{
    //If you are in the main menu, don't allow the game speed to be updated
    if(APC_Base::IsInMainMenu())
    {
        return;
    }

    //Update the game speed. If it is already sped up, slow it down
    if(UGameplayStatics::GetGlobalTimeDilation(GetWorld()) > 1.f)
    {
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
    }
    else
    {
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2.5);
    }
}

void APC_Base::UpdatePause()
{
    //Update the pause state
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        if (IsPaused() && !APC_Base::IsInMainMenu())
        {
            GameStateInterface->Execute_PauseGame(GetWorld()->GetGameState(), false);
        }
        else
        {
            GameStateInterface->Execute_PauseGame(GetWorld()->GetGameState(), true);
        }
    }
}

bool APC_Base::IsInMainMenu()
{
    //Compare the current name of the level to see if we are in the main menu
    return (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "MainMenu");
}

void APC_Base::PassTowerToConstruct_Implementation(FName TowerRowName)
{
    //Get the proper class from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //and pass it to the Tower construct function
    II_Grid* GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->ConstructTower(TowerStats->TowerBlueprint);

        //Subtract the cost of the tower from the current resources
        II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
        if (GameStateInterface)
        {
            GameStateInterface->SetResources(-TowerStats->TowerCost);
        }
    }
    //When all is done, call the function to allow the player to move again
    APC_Base::OnCancelButtonDown();
}

void APC_Base::PassTowerRange_Implementation(FName TowerRowName)
{
    //Get the proper class from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //and pass its range to the grid to display the decal
    II_Grid* GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->ConstructRangeDecal(TowerStats->TowerRange);
    }
}

void APC_Base::PassTowerSell_Implementation(FName TowerRowName, AActor* TowerToSell)
{
    //Get the cost from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(TowerRowName, ContextString, true);

    //Call the grid and tell it to sell this tower
    II_Grid* GridInterface = Cast<II_Grid>(CurrentlySelectedGrid);
    if (GridInterface)
    {
        GridInterface->SellTower(TowerToSell);

        //Update the resources with the sell value of the tower
        II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
        if (GameStateInterface)
        {
            int SellValue = ((TowerStats->TowerCost / 5) * 3);
            GameStateInterface->SetResources(SellValue);
        }
    }

    //Once it is sold, hide the menu again
    APC_Base::OnCancelButtonDown();
}
