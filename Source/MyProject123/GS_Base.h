// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "I_BaseGameState.h"
#include "I_EnemySpawner.h"
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

private:
    //The grids that are available in the map
    UPROPERTY()
    TArray<AActor*> TowerGrids;

    //The goal the enemies will move towards
    UPROPERTY()
    FVector EnemyTargetGoal;

    //The current lives available to the player
    UPROPERTY()
    int CurrentLives;

    //The current resources used to buy towers available to the player
    UPROPERTY()
    int CurrentResources;

    //The current wave
    UPROPERTY()
    int CurrentWave;

    //The amount of units in this wave
    UPROPERTY()
    int TotalUnitsInWave;

    //The total amount of waves to cycle through
    UPROPERTY()
    int TotalWaves;

    UPROPERTY()
    FTimerHandle NextWaveTimer;

    UPROPERTY()
    TArray<AActor*> AllEnemySpawners;

    UPROPERTY()
    bool FirstTower = false;

public:
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

    UPROPERTY(BlueprintAssignable, Category = "Base Game State")
    FGamePausedDelegate OnGamePaused;

    UPROPERTY(EditAnywhere)
    class UDataTable* LevelData;

    //Interface Functions
    virtual void SetGrid(AActor* Grid) override;
    virtual TArray<AActor*> GetGrids() override;
    virtual void SetEnemyGoal(FVector EnemyGoal) override;
    virtual FVector GetEnemyGoal() override;
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
