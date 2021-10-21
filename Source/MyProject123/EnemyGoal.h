// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GS_Base.h"
#include "GameFramework/Actor.h"
#include "I_BaseGameState.h"
#include "I_Enemy.h"
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

    //When an actor overlaps with the box component
    UFUNCTION()
    virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    //Used to check for overlapping enemy actors
    UPROPERTY(VisibleAnywhere, Category = "Enemy Goal")
    class UBoxComponent* BoxCollision;
};
