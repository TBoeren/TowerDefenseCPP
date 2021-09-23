// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Base.h"

void AGS_Base::SetGrid(AActor* Grid)
{
    TowerGrids.Add(Grid);
}

void AGS_Base::GetGrids(TArray<AActor*> &Grids)
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