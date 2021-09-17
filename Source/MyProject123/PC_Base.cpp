// Fill out your copyright notice in the Description page of Project Settings.
#include "PC_Base.h"

APC_Base::APC_Base()
{
}

void APC_Base::SetupInputComponent()
{
    Super::SetupInputComponent();
    //Bind the inputs of the mouse to the function to call

    InputComponent->BindAxis(TEXT("MouseMove"), this, &APC_Base::MouseMove);
    InputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &APC_Base::CameraZoomIn);
    InputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &APC_Base::CameraZoomOut);
}

void APC_Base::MouseMove(float Value)
{
    //When the mouse moves,
    FVector2D MousePosition;

    //Set the mouse position and move the camera if near the edge
    if (GetMousePosition(MousePosition.X, MousePosition.Y) && !IsInputKeyDown(EKeys::LeftMouseButton))
    {
        II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
        if(PawnInterface)
        {
            PawnInterface->EdgeScrollCamera(APC_Base::CalculateEdgeScroll(MousePosition));
        }       
    }

    AActor* Grid;
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState()); 
	if(GameStateInterface)
	{
		GameStateInterface->GetGrid(Grid);
	}
    else
    {
        return;
    }

    II_Grid* GridInterface = Cast<II_Grid>(Grid);
    if(GridInterface)
    {
        //Check if you are on the grid
        FHitResult Result;
        if(GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Result))
        {
            //Convert the location of the mouse to the grid location
            int Row;
            int Column;

            if(GridInterface->LocationToTile(Result.Location, Row, Column))
            {
                //If the location is on the grid, highlight the appropriate tile
                GridInterface->SetSelectedTile(Row, Column);
            }
        }
        else
        {
            //If it is not on the grid, pass a false value to set the visibility back to hidden
            GridInterface->SetSelectedTile(-1, -1);
        }
    }
}

FVector2D APC_Base::CalculateEdgeScroll(FVector2D MousePosition)
{
    FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    float XValue = ((MousePosition.X / ViewportSize.X) - 0.5);
    float YValue = ((MousePosition.Y / ViewportSize.Y) - 0.5) * -1;

    if(FMath::Abs(XValue) > EdgeScrollTolerance || FMath::Abs(YValue) > EdgeScrollTolerance)
    {
        return FVector2D((YValue * EdgeScrollSpeed), (XValue * EdgeScrollSpeed));
    }

    return FVector2D(0,0);
}

void APC_Base::CameraZoomIn()
{
    II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
    if(PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * -1.0));
    }
}

void APC_Base::CameraZoomOut()
{
    II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
    if(PawnInterface)
    {
        PawnInterface->UpdateCameraBoomLength((ZoomSpeed * 1.0));
    }
}

