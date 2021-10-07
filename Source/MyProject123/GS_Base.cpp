// Fill out your copyright notice in the Description page of Project Settings.

#include "GS_Base.h"

void AGS_Base::BeginPlay()
{
    Super::BeginPlay();

    //get the resources gained upon death
    static const FString ContextString(TEXT("Tower Data"));
    FMatchStats *MatchStats = LevelData->FindRow<FMatchStats>(FName(GetWorld()->GetName()), ContextString, true);
    if (MatchStats)
    {
        //Add it to the starter resources
        AGS_Base::SetResources(MatchStats->StartOfMatchResources);
        AGS_Base::SetLives(MatchStats->StartOfMatchLives);
    }

    //Set the current wave to 0
    AGS_Base::SetCurrentWave(0);

    //And calculate the total waves and set it 
    TotalWaves = AGS_Base::CalculateTotalWaves();
}

void AGS_Base::StartNextWaveCountdown(int Seconds)
{
    //Start a timer for the start next wave function
    if (GetWorldTimerManager().TimerExists(NextWaveTimer))
	{
		GetWorldTimerManager().ClearTimer(NextWaveTimer);
	}

    GetWorldTimerManager().SetTimer(NextWaveTimer, this, &AGS_Base::StartNextWave, (float)Seconds, false);

    //Call the event dispatcher to inform the UI of the countdown
    OnNextWaveCountdownStarted.Broadcast(Seconds);
}

void AGS_Base::StartNextWave()
{
    //Once all is prepared, start the enemyspawners
    for(AActor* EnemySpawner : AllEnemySpawners)
    {
        II_EnemySpawner* EnemySpawnerInterface = Cast<II_EnemySpawner>(EnemySpawner);
        if(EnemySpawnerInterface)
        {
            EnemySpawnerInterface->StartNextWave();
            SetTotalUnitsInWave(EnemySpawnerInterface->CalculateTotalEnemies());
        }
    }
}

void AGS_Base::SetGrid(AActor *Grid)
{
    TowerGrids.Add(Grid);
}

void AGS_Base::GetGrids(TArray<AActor *> &Grids)
{
    Grids = TowerGrids;
}

void AGS_Base::SetEnemyGoal(FVector EnemyGoal)
{
    EnemyTargetGoal = EnemyGoal;
}

void AGS_Base::GetEnemyGoal(FVector &EnemyGoal)
{
    EnemyGoal = EnemyTargetGoal;
}

void AGS_Base::SetLives(int Lives)
{
    CurrentLives += Lives;
    OnLivesUpdated.Broadcast(CurrentLives);
}

void AGS_Base::SetResources(int Resources)
{
    CurrentResources += Resources;
    OnResourcesUpdated.Broadcast(CurrentResources);
}

int AGS_Base::GetLivesPure()
{
    return CurrentLives;
}

int AGS_Base::GetResourcesPure()
{
    return CurrentResources;
}

int AGS_Base::GetLives_Implementation()
{
    return CurrentLives;
}

int AGS_Base::GetResources_Implementation()
{
    return CurrentResources;
}

TArray<FName> AGS_Base::GetAvailableTowers_Implementation()
{
    TArray<FName> temp;
    static const FString ContextString(TEXT("Tower Data"));
    FMatchStats *MatchStats = LevelData->FindRow<FMatchStats>(FName(GetWorld()->GetName()), ContextString, true);
    if (MatchStats)
    {
        return MatchStats->AvailableTowers;
    }

    return temp;
}

void AGS_Base::SetCurrentWave(int Wave)
{
    CurrentWave = Wave;
    if(CurrentWave == TotalWaves)
    {
        OnAllWavesCompleted.Broadcast(CurrentWave);
    }
    else if(CurrentWave == 0)
    {
        OnCurrentWaveUpdated.Broadcast(CurrentWave);
    } 
    else
    {
        StartNextWaveCountdown(10);
        OnCurrentWaveUpdated.Broadcast(CurrentWave);
    }
}

void AGS_Base::SetTotalUnitsInWave(int TotalUnits)
{
    TotalUnitsInWave += TotalUnits;

    if(IsWaveOver())
    {
        SetCurrentWave((CurrentWave + 1));
    }
}

bool AGS_Base::IsWaveOver()
{
    if(TotalUnitsInWave == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int AGS_Base::CalculateTotalWaves()
{
    int LTotalWaves = 0;
    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundActors);

    for(AActor* Actor : FoundActors)
    {             
        //Add the spawner to an array for future use
        AllEnemySpawners.Add(Actor);

        int LRowLength = 0;
        II_EnemySpawner* EnemySpawnerInterface = Cast<II_EnemySpawner>(Actor);
        if(EnemySpawnerInterface)
        {
            LRowLength = EnemySpawnerInterface->GetTotalWavesInSpawner();
        }       

        if(LRowLength > LTotalWaves)
        {
            LTotalWaves = LRowLength;
        }
    }
    
    return LTotalWaves;
}

int AGS_Base::GetTotalWavesPure()
{
    return TotalWaves;
}

int AGS_Base::GetTotalWaves_Implementation()
{
    return TotalWaves;
}

bool AGS_Base::FirstTowerPlaced()
{
    if(!FirstTower)
    {
        FirstTower = true;
        StartNextWaveCountdown(10);
        return false;
    }
    else
    {
        return true;
    }
}

void AGS_Base::PauseGame_Implementation(bool Pause)
{
    // Unpauses/Pauses Game
    UGameplayStatics::SetGamePaused(GetWorld(), Pause);
    OnGamePaused.Broadcast(Pause);
}