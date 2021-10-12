// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "GS_Base.h"
#include "I_BaseGameState.h"
#include "EnemyController.generated.h"

UCLASS()
class MYPROJECT123_API AEnemyController : public AAIController
{
    GENERATED_BODY()

    AEnemyController();

protected:
    // Called when the game starts or when spawned
    virtual void OnPossess(APawn* InPawn) override;

public:
    UPROPERTY(EditAnywhere, Category = "Base EnemyController|Movement")
    FVector GoalLocation;
};
