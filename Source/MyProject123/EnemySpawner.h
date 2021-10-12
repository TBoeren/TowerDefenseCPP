// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseEnemy.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "I_BaseGameState.h"
#include "I_EnemySpawner.h"
#include "TimerManager.h"
#include "EnemySpawner.generated.h"

USTRUCT(BlueprintType)
struct FWaveStats
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ABaseEnemy> EnemyBlueprint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int AmountToSpawn;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TimeBetweenSpawns;
};

USTRUCT(BlueprintType)
struct FWaveContainer : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FWaveStats> UnitsInWave;
};

UCLASS()
class MYPROJECT123_API AEnemySpawner : public AActor, public II_EnemySpawner
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AEnemySpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	//Function responsable for spawning the units
    UFUNCTION()
    void SpawnUnits(FWaveStats CurrentUnitsInWave, FName CurrentRowName);

	//Function called when the wave is completed
    UFUNCTION()
    void CompleteWave();

	//The current wave
    UPROPERTY(EditAnywhere, Category = "Enemy Spawner")
    int CurrentWave;

	//THe index used to decide which enemy in the array to spawn
    UPROPERTY()
    int WaveInWaveIndex;

	//The amount of units of this type that have been spawned
    UPROPERTY()
    int CurrentlySpawnedUnits;

public:
	//The data table that is used to spawn the waves 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawner")
    class UDataTable* WaveData;

	//The timer responsable for calling the SpawnUnit function
    UPROPERTY(BlueprintReadWrite, Category = "Enemy Spawner|Timer")
    FTimerHandle UnitSpawnTimer;

    UFUNCTION()
    void OnCurrentWaveUpdated(int Wave);

    //Interface functions
    virtual int GetTotalWavesInSpawner() override;
    virtual void StartNextWave() override;
    virtual int CalculateTotalEnemies() override;
};
