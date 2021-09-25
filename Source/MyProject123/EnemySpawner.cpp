// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//Bind the event dispatcher to update the variable of the current wave
	AGS_Base* GameState = Cast<AGS_Base>(GetWorld()->GetGameState());
	if(GameState->IsValidLowLevel())
	{
		GameState->OnCurrentWaveUpdated.AddDynamic(this, &AEnemySpawner::OnCurrentWaveUpdated);
	}

	//Calculate the total amount of enemies that will spawn in the current wave
	TArray<FName> RowNames = WaveData->GetRowNames();
	FName CurrentRowName = RowNames[CurrentWave];

	//Use that to find the appropriate wave information
	static const FString ContextString(TEXT("Tower Data"));
	FWaveContainer* WaveStats = WaveData->FindRow<FWaveContainer>(FName(CurrentRowName), ContextString, true);
	int TotalUnitsInWave = 0;

	for(FWaveStats Wave : WaveStats->UnitsInWave)
	{
		TotalUnitsInWave += Wave.AmountToSpawn;
	}
	
	//Pass this total to the game state
	II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->SetTotalUnitsInWave(TotalUnitsInWave);
    }
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::StartNextWave()
{
	// if a timer exists, clear it TODO move to the complete function?
	if(GetWorldTimerManager().TimerExists(UnitSpawnTimer))
    {
		GetWorldTimerManager().ClearTimer(UnitSpawnTimer);
    }

	//Clear CurrentlySpawnedUnits
	CurrentlySpawnedUnits = 0;

	//Get all the row names and find the one that belongs to this wave.
	TArray<FName> RowNames = WaveData->GetRowNames();
	FName CurrentRowName = RowNames[CurrentWave];

	//Use that to find the appropriate wave information
	static const FString ContextString(TEXT("Tower Data"));
	FWaveContainer* WaveStats = WaveData->FindRow<FWaveContainer>(FName(CurrentRowName), ContextString, true);

	//From the array of wave information, get the wave information that is associated with the index
	FWaveStats CurrentUnitsInWave = WaveStats->UnitsInWave[WaveInWaveIndex];

	//Set the wave information in the timer delegate
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("SpawnUnits"), CurrentUnitsInWave, CurrentRowName);

    GetWorldTimerManager().SetTimer(UnitSpawnTimer, TimerDelegate, CurrentUnitsInWave.TimeBetweenSpawns, true);
}

void AEnemySpawner::SpawnUnits(FWaveStats CurrentUnitsInWave, FName CurrentRowName)
{
	//Spawn the unit
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ABaseEnemy* SpawnedUnit = GetWorld()->SpawnActor<ABaseEnemy>(CurrentUnitsInWave.EnemyBlueprint, GetActorLocation(), Rotation, SpawnInfo);

	//Increment total unit spawned
	CurrentlySpawnedUnits++;

	//Check if the amount of units spawned is equal to the amount to spawn
	if(CurrentlySpawnedUnits == CurrentUnitsInWave.AmountToSpawn)
	{
		// if it is, increment the WaveInWaveIndex 
		WaveInWaveIndex++;

		//check if it is greater than the length of (array-1)
		static const FString ContextString(TEXT("Tower Data"));
		FWaveContainer* WaveStats = WaveData->FindRow<FWaveContainer>(FName(CurrentRowName), ContextString, true);

		if(WaveInWaveIndex > (WaveStats->UnitsInWave.Num() - 1))
		{
			//if it is, set the WaveInWaveIndex to 0 and call the complete wave function
			WaveInWaveIndex = 0;
			AEnemySpawner::CompleteWave();
		}
		else
		{
			//Otherwise, call the startnextwave function
			StartNextWave();
		}
	}
}

void AEnemySpawner::CompleteWave()
{
	// if a timer exists, clear it 
	if(GetWorldTimerManager().TimerExists(UnitSpawnTimer))
    {
		GetWorldTimerManager().ClearTimer(UnitSpawnTimer);
    }
}

void AEnemySpawner::OnCurrentWaveUpdated(int Wave)
{
	CurrentWave = Wave;
}