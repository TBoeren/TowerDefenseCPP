// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "BaseEnemy.h"
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

	UFUNCTION()
	void SpawnUnits(FWaveStats CurrentUnitsInWave, FName CurrentRowName);

	UFUNCTION()
	void CompleteWave();

	UPROPERTY(EditAnywhere)
	int CurrentWave;

	UPROPERTY()
	int WaveInWaveIndex;

	UPROPERTY()
	int CurrentlySpawnedUnits;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UDataTable* WaveData;

	UPROPERTY(BlueprintReadWrite, Category = "Base Player Pawn|Camera")
	FTimerHandle UnitSpawnTimer;

	UFUNCTION()
	void OnCurrentWaveUpdated(int Wave);

	//Interface functions
	virtual int GetTotalWavesInSpawner() override;
	virtual void StartNextWave() override;
	virtual int CalculateTotalEnemies() override;

};
