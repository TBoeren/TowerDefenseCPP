// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/DataTable.h"
#include "I_BaseGameState.h"
#include "GS_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStateDelegate, int, Current);

USTRUCT(BlueprintType)
struct FMatchStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartOfMatchLives;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int StartOfMatchResources;
};

UCLASS()
class MYPROJECT123_API AGS_Base : public AGameStateBase, public II_BaseGameState
{
	GENERATED_BODY()
	
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:

	UPROPERTY()
	TArray<AActor*> TowerGrids;

	UPROPERTY()
	FVector EnemyTargetGoal;

	UPROPERTY()
	int CurrentLives;

	UPROPERTY()
	int CurrentResources;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Resources")
	FGameStateDelegate OnLivesUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Base Game State | Resources")
	FGameStateDelegate OnResourcesUpdated;

	UPROPERTY(EditAnywhere)
	class UDataTable *LevelData;

	virtual void SetGrid(AActor* Grid) override;
	virtual void GetGrids(TArray<AActor*> &Grids) override;
	virtual void SetEnemyGoal(FVector EnemyGoal) override;
	virtual void GetEnemyGoal(FVector &EnemyGoal) override;
	virtual void SetLives(int Lives) override;
	virtual void SetResources(int Resources) override;
	virtual int GetLivesPure() override;
	virtual int GetResourcesPure() override;
	int GetLives_Implementation() override;
	int GetResources_Implementation() override;

};
