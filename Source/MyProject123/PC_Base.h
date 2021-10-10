// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/DataTable.h"
#include "I_BasePawn.h"
#include "I_Grid.h"
#include "I_BaseGameState.h"
#include "I_BasePlayerController.h"
#include "GS_Base.h"
#include "TowerBase.h"
#include "PC_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTowerDelegate, FIntPoint, SelectedTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildingDelegate, AActor*, SelectedTower);

UCLASS()
class MYPROJECT123_API APC_Base : public APlayerController, public II_BasePlayerController
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

	UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Tower Placement")
	FIntPoint CurrentTile;

	UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Grid")
	TArray<AActor*> Grids;

	UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Grid")
	AActor* CurrentlySelectedGrid;

	UPROPERTY(EditAnywhere)
	class UDataTable* TowerData;

	UPROPERTY()
	bool LocationSelected = false;
	
public:
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Placement")
	FTowerDelegate OnTileSelected;

	UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Placement")
	FTowerDelegate OnTileUnselected;

	UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Selection")
	FBuildingDelegate OnTowerSelected;

private:
	UFUNCTION()
	void MouseMove(float Value);

	UFUNCTION()
	void OnSelectButtonDown();

	UFUNCTION()
	void OnCancelButtonDown();

	UFUNCTION()
	void CameraZoomIn();

	UFUNCTION()
	void CameraZoomOut();
	
	UFUNCTION()
	FVector2D CalculateEdgeScroll(FVector2D MousePosition);

	void PassTowerToConstruct_Implementation(FName TowerRowName) override;
	void PassTowerRange_Implementation(FName TowerRowName) override;
	void PassTowerSell_Implementation(FName TowerRowName, AActor* TowerToSell) override;
};
