// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_Base.h"

void AGS_Base::SetGrid(AActor* Grid)
{
    TowerGrid = Grid;
}

void AGS_Base::GetGrid(AActor* &Grid)
{
    Grid = TowerGrid;
}