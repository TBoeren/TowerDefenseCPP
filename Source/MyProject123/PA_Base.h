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

	void SetCameraBoomLength();

	UPROPERTY(EditAnywhere, Category = "Base Player Pawn|Settings")
	float MinCameraBoomLength;

	UPROPERTY(EditAnywhere, Category = "Base Player Pawn|Settings")
	float MaxCameraBoomLength;

	UPROPERTY(BlueprintReadWrite, Category = "Base Player Pawn|Camera")
	float TargetCameraBoomLength;

	UPROPERTY(BlueprintReadWrite, Category = "Base Player Pawn|Camera")
	FTimerHandle CameraZoomTimer;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent *BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent *CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent *TopDownCamera;

	virtual void EdgeScrollCamera(FVector2D XYValue) override; //c++ implementation for BP look into _Implementation
	virtual void UpdateCameraBoomLength(float Value) override;
};
