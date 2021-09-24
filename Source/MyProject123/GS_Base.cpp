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