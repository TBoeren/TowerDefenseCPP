// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyController::AEnemyController()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    //Call to the game state interface and get the goal for the enemy to move towards
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->GetEnemyGoal(GoalLocation);
        MoveTo(GoalLocation);
    }
    else
    {
        return;
    }
}