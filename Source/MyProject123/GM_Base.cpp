// Fill out your copyright notice in the Description page of Project Settings.

#include "GM_Base.h"
#include "GS_Base.h"
#include "PS_Base.h"
#include "PC_Base.h"
#include "PA_Base.h"
#include "HUD_Base.h"
#include "EnemySpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AGM_Base::AGM_Base()
{
    GameStateClass = AGS_Base::StaticClass();
    PlayerStateClass = APS_Base::StaticClass();
    PlayerControllerClass = APC_Base::StaticClass();
    DefaultPawnClass = APA_Base::StaticClass();
    HUDClass = AHUD_Base::StaticClass();
}

void AGM_Base::BeginPlay()
{
    Super::BeginPlay();

    II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
	if (GameStateInterface)
	{
		GameStateInterface->SetTotalWaves(AGM_Base::CalculateTotalWaves());
	}
}

int AGM_Base::CalculateTotalWaves()
{
    int TotalWaves = 0;
    TArray<AActor*> FoundActors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundActors);

    for(AActor* Actor : FoundActors)
    {
        AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(Actor);
        int LRowLength = EnemySpawner->WaveData->GetRowNames().Num();

        if(LRowLength > TotalWaves)
        {
            TotalWaves = LRowLength;
        }
    }
    
    return TotalWaves;
}