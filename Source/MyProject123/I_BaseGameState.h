// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_BaseGameState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_BaseGameState : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_BaseGameState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void SetGrid(AActor* Grid) = 0;

	UFUNCTION()
	virtual void GetGrids(TArray<AActor*> &Grids) = 0;

	UFUNCTION()
	virtual void SetEnemyGoal(FVector EnemyGoal) = 0;

	UFUNCTION()
	virtual void GetEnemyGoal(FVector &EnemyGoal) = 0;

	UFUNCTION()
	virtual void SetLives(int Lives) = 0;

	UFUNCTION()
	virtual void SetResources(int Resources) = 0;

	UFUNCTION()
	virtual int GetLivesPure() = 0;

	UFUNCTION()
	virtual int GetResourcesPure() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
	int GetLives();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
	int GetResources();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
	TArray<FName> GetAvailableTowers();

	UFUNCTION()
	virtual void SetCurrentWave(int Wave) = 0;

	UFUNCTION()
	virtual void SetTotalUnitsInWave(int TotalUnits) = 0;

	UFUNCTION()
	virtual int GetTotalWavesPure() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wave Management")
	int GetTotalWaves();

	UFUNCTION()
	virtual void StartNextWaveCountdown(int Seconds) = 0;

	UFUNCTION()
	virtual bool FirstTowerPlaced() = 0;
};
