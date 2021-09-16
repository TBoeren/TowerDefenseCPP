// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Base.h"

APC_Base::APC_Base()
{
}

void APC_Base::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    InputComponent->BindAxis(TEXT("MouseMove"), this, &APC_Base::MouseMove);
    InputComponent->BindAction(TEXT("CameraZoomIn"), IE_Pressed, this, &APC_Base::CameraZoomIn);
    InputComponent->BindAction(TEXT("CameraZoomOut"), IE_Pressed, this, &APC_Base::CameraZoomOut);
}

void APC_Base::MouseMove(float Value)
{
    FVector2D MousePosition;

    if (GetMousePosition(MousePosition.X, MousePosition.Y) && !IsInputKeyDown(EKeys::LeftMouseButton))
    {
        II_BasePawn* PawnInterface = Cast<II_BasePawn>(GetPawn());
        if(PawnInterface)
        {
            PawnInterface->EdgeScrollCamera(APC_Base::CalculateEdgeScroll(MousePosition));
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

