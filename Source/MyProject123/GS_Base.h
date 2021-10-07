// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/DataTable.h"
#include "I_BaseGameState.h"
#include "I_EnemySpawner.h"
#include "EnemySpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GS_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateDelegate, int, Current);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePausedDelegate, bool, Paused);

USTRUCT(BlueprintType)
struct FMatchStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartOfMatchLives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartOfMatchResources;

	UPROPERTY(EditAnywhere)
	TArray<FName> AvailableTowers;
};

UCLASS()
class MYPROJECT123_API AGS_Base : public AGameStateBase, public II_BaseGameState
{
	GENERATED_BODY()
	
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	bool IsWaveOver();

	UFUNCTION()
	int CalculateTotalWaves();

	UFUNCTION()
	void StartNextWave();

	public:

	UPROPERTY()
	TArray<AActor*> TowerGrids;

	UPROPERTY()
	FVector EnemyTargetGoal;

	UPROPERTY()
	int CurrentLives;

	UPROPERTY()
	int CurrentResources;

	UPROPERTY()
	int CurrentWave;

	UPROPERTY()
	int TotalUnitsInWave;

	UPROPERTY()
	int TotalWaves;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Resources")
	FGameStateDelegate OnLivesUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Resources")
	FGameStateDelegate OnResourcesUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Waves")
	FGameStateDelegate OnCurrentWaveUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Waves")
	FGameStateDelegate OnNextWaveCountdownStarted;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Waves")
	FGameStateDelegate OnAllWavesCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Placement")
	FGamePausedDelegate OnGamePaused;

	UPROPERTY()
	FTimerHandle NextWaveTimer;

	UPROPERTY(EditAnywhere)
	class UDataTable *LevelData;

	UPROPERTY()
	TArray<AActor*> AllEnemySpawners;

	UPROPERTY()
	bool FirstTower = false;

	virtual void SetGrid(AActor* Grid) override;
	virtual void GetGrids(TArray<AActor*> &Grids) override;

	virtual void SetEnemyGoal(FVector EnemyGoal) override;
	virtual void GetEnemyGoal(FVector &EnemyGoal) override;

	virtual void SetLives(int Lives) override;
	virtual int GetLivesPure() override;
	int GetLives_Implementation() override;

	virtual void SetResources(int Resources) override;
	int GetResources_Implementation() override;
	virtual int GetResourcesPure() override;

	TArray<FName> GetAvailableTowers_Implementation() override;

	virtual void SetCurrentWave(int Wave) override;
	
	virtual void SetTotalUnitsInWave(int TotalUnits) override;

	virtual int GetTotalWavesPure() override;
	int GetTotalWaves_Implementation() override;

	virtual void StartNextWaveCountdown(int Seconds) override;
	virtual bool FirstTowerPlaced() override;

	void PauseGame_Implementation(bool Pause) override;
};
