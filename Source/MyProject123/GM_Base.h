// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "I_BaseGameState.h"
#include "GM_Base.generated.h"

UCLASS()
class MYPROJECT123_API AGM_Base : public AGameModeBase
{
	GENERATED_BODY()

	AGM_Base();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:

	UFUNCTION()
	int CalculateTotalWaves();
};
