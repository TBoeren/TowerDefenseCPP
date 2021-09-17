// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "I_BaseGameState.h"
#include "GS_Base.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT123_API AGS_Base : public AGameStateBase, public II_BaseGameState
{
	GENERATED_BODY()
	
	public:

	UPROPERTY()
	AActor* TowerGrid;

	virtual void SetGrid(AActor* Grid) override;
	virtual void GetGrid(AActor* &Grid) override;

};
