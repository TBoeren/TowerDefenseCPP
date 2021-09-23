// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "I_BaseGameState.h"
#include "GS_Base.h"
#include "EnemyGoal.generated.h"

UCLASS()
class MYPROJECT123_API AEnemyGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FVector EnemyGoal;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
