// Fill out your copyright notice in the Description page of Project Settings.
#include "TowerBase.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the attack range 
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Collision"));
	RootComponent = SphereCollision;

    //Create the tower mesh
    TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Get the stats for this tower
	static const FString ContextString(TEXT("Tower Data"));
	FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(FName(RowName), ContextString, true);
	if(TowerStats)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *TowerStats->TowerName);
	}
}

// Called every frame
void ATowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

