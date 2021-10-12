// Fill out your copyright notice in the Description page of Project Settings.

#include "PA_Base.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APA_Base::APA_Base()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Create the bounds collision
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Bounds Collision"));

    //Create the camera boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
    CameraBoom->AttachToComponent(BoxCollision, FAttachmentTransformRules::KeepRelativeTransform);

    //Create the top down camera and attach it to the camera boom
    TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Top Down Camera"));
    TopDownCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void APA_Base::BeginPlay()
{
    Super::BeginPlay();

    //Set the camera boom length to the max
    CameraBoom->TargetArmLength = MaxCameraBoomLength;

    //Create the input mode
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

    //Set the input mode
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
}

// Called to bind functionality to input
void APA_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APA_Base::EdgeScrollCamera(FVector2D XYValue)
{
    //Add a local offset to the box collision to move it
    FHitResult* LHitResult = nullptr;
    BoxCollision->AddLocalOffset(FVector(XYValue.X, XYValue.Y, 0), true, LHitResult, ETeleportType::None);
}

void APA_Base::UpdateCameraBoomLength(float Value)
{
    float TargetCameraBoomLength = FMath::Clamp((CameraBoom->TargetArmLength + Value), MinCameraBoomLength, MaxCameraBoomLength);

    if (!GetWorldTimerManager().TimerExists(CameraZoomTimer))
    {
        //Set the target arm length information in the timer delegate
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("SetCameraBoomLength"), TargetCameraBoomLength);
        GetWorldTimerManager().SetTimer(CameraZoomTimer, TimerDelegate, 0.01, true);
    }
}

void APA_Base::SetCameraBoomLength(float TargetArmLength)
{
    CameraBoom->TargetArmLength = FMath::FInterpConstantTo(CameraBoom->TargetArmLength, TargetArmLength, GetWorld()->DeltaTimeSeconds, 300);

    if (CameraBoom->TargetArmLength == TargetArmLength)
    {
        GetWorldTimerManager().ClearTimer(CameraZoomTimer);
    }
}