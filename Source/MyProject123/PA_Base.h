// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "I_BasePawn.h"
#include "TimerManager.h"
#include "PA_Base.generated.h"

UCLASS()
class MYPROJECT123_API APA_Base : public APawn, public II_BasePawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    APA_Base();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //A function to interp the length of the camera boom
    UFUNCTION()
    void SetCameraBoomLength(float TargetArmLength);

    //The minimum level of zoom
    UPROPERTY(EditAnywhere, Category = "Base Player Pawn|Settings")
    float MinCameraBoomLength;

    //The maximum level of zoom
    UPROPERTY(EditAnywhere, Category = "Base Player Pawn|Settings")
    float MaxCameraBoomLength;

    UPROPERTY(BlueprintReadWrite, Category = "Base Player Pawn|Camera")
    FTimerHandle CameraZoomTimer;

public:
    // Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    //Collision box used to constrain the camera in the level
    UPROPERTY(VisibleAnywhere, Category = "Base Player Pawn")
    class UBoxComponent* BoxCollision;

    //Camera boom used for zooming the camera
    UPROPERTY(VisibleAnywhere, Category = "Base Player Pawn|Camera")
    class USpringArmComponent* CameraBoom;

    //Main Camera
    UPROPERTY(VisibleAnywhere, Category = "Base Player Pawn|Camera")
    class UCameraComponent* TopDownCamera;

    //Interface functions
    virtual void EdgeScrollCamera(FVector2D XYValue) override;
    virtual void UpdateCameraBoomLength(float Value) override;
};
