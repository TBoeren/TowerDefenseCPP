// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "I_BasePawn.h"
#include "PC_Base.generated.h"

UCLASS()
class MYPROJECT123_API APC_Base : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APC_Base();

protected:

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float EdgeScrollTolerance;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float EdgeScrollSpeed;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float ZoomSpeed;
	
public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void MouseMove(float Value);

	UFUNCTION()
	void CameraZoomIn();

	UFUNCTION()
	void CameraZoomOut();

	
	UFUNCTION()
	FVector2D CalculateEdgeScroll(FVector2D MousePosition);
};
