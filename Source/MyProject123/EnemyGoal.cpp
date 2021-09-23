// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGoal.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyGoal::AEnemyGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyGoal::BeginPlay()
{
	Super::BeginPlay();
	
	//Set the enemy goal to the actors current location 
	II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
	if (GameStateInterface)
	{
		GameStateInterface->SetEnemyGoal(GetActorLocation());
	}
}

// Called every frame
void AEnemyGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

