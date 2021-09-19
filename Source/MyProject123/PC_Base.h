// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "I_BasePawn.h"
#include "I_Grid.h"
#include "I_BaseGameState.h"
#include "GS_Base.h"
#include "TowerBase.h"
#include "PC_Base.generated.h"

UCLASS()
class MYPROJECT123_API APC_Base : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APC_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float EdgeScrollTolerance;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float EdgeScrollSpeed;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, Category = "Base PlayerController|Tower Placement")
	TSubclassOf<ATowerBase> TowerToSpawn;

	UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Tower Placement")
	FIntPoint CurrentTile;

	UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Grid")
	AActor* Grid;
	
public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void MouseMove(float Value);

	UFUNCTION()
	void OnMouseButtonDown();

	UFUNCTION()
	void CameraZoomIn();

	UFUNCTION()
	void CameraZoomOut();
	
	UFUNCTION()
	FVector2D CalculateEdgeScroll(FVector2D MousePosition);
};
