// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"
#include "GS_Base.h"

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

    //Bind the event dispatcher to update when the variable of the current wave changes
    AGS_Base* GameState = Cast<AGS_Base>(GetWorld()->GetGameState());
    if (GameState->IsValidLowLevel())
    {
        GameState->OnCurrentWaveUpdated.AddDynamic(this, &AEnemySpawner::OnCurrentWaveUpdated);
    }
}

void AEnemySpawner::StartNextWave()
{
    // if a timer exists, clear it
    if (GetWorldTimerManager().TimerExists(UnitSpawnTimer))
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
    if (WaveStats && WaveStats->UnitsInWave.Num() > 0)
    {
        //From the array of wave information, get the wave information that is associated with the index
        FWaveStats CurrentUnitsInWave = WaveStats->UnitsInWave[WaveInWaveIndex];

        //Set the wave information in the timer delegate
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("SpawnUnits"), CurrentUnitsInWave, CurrentRowName);

        //Start spawning units with the wave information
        GetWorldTimerManager().SetTimer(UnitSpawnTimer, TimerDelegate, CurrentUnitsInWave.TimeBetweenSpawns, true);
    }
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
    if (CurrentlySpawnedUnits == CurrentUnitsInWave.AmountToSpawn)
    {
        // if it is, increment the WaveInWaveIndex
        WaveInWaveIndex++;

        //check if it is greater than the length of (array-1)
        static const FString ContextString(TEXT("Tower Data"));
        FWaveContainer* WaveStats = WaveData->FindRow<FWaveContainer>(FName(CurrentRowName), ContextString, true);

        if (WaveInWaveIndex > (WaveStats->UnitsInWave.Num() - 1))
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
    if (GetWorldTimerManager().TimerExists(UnitSpawnTimer))
    {
        GetWorldTimerManager().ClearTimer(UnitSpawnTimer);
    }
}

void AEnemySpawner::OnCurrentWaveUpdated(int Wave)
{
    //When the current wave is updated, set CurrentWave to that value
    CurrentWave = Wave;
}

int AEnemySpawner::GetTotalWavesInSpawner()
{
    //Get the amount of waves in the spawner
    return WaveData->GetRowNames().Num();
}

int AEnemySpawner::CalculateTotalEnemies()
{
    //Get the current wave
    TArray<FName> RowNames = WaveData->GetRowNames();
    FName CurrentRowName = RowNames[CurrentWave];

    //Use that to find the appropriate wave information
    static const FString ContextString(TEXT("Tower Data"));
    FWaveContainer* WaveStats = WaveData->FindRow<FWaveContainer>(FName(CurrentRowName), ContextString, true);
    if (WaveStats && WaveStats->UnitsInWave.Num() > 0)
    {
        int TotalUnitsInWave = 0;

        for (FWaveStats Wave : WaveStats->UnitsInWave)
        {
            //Calculate the total amount of enemies that will spawn in the current wave
            TotalUnitsInWave += Wave.AmountToSpawn;
        }

        return TotalUnitsInWave;
    }

    return 0;
}