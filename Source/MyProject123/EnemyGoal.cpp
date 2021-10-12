// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyGoal.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyGoal::AEnemyGoal()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //Create the end collision box
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Attack Range Collision"));
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyGoal::OnOverlapBegin);
    RootComponent = BoxCollision;
}

// Called when the game starts or when spawned
void AEnemyGoal::BeginPlay()
{
    Super::BeginPlay();

    //Call to the game state interface and set the enemy goal to the actors current location
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->SetEnemyGoal(GetActorLocation());
    }
}

void AEnemyGoal::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //When an enemy overlaps, remove a life
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface)
    {
        GameStateInterface->SetLives(-1);
    }

	//Kill the enemy that overlaps with the box
    II_Enemy* EnemyInterface = Cast<II_Enemy>(OtherActor);
    if (EnemyInterface)
    {
        EnemyInterface->Death(false);
    }
}