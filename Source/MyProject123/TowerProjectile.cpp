// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerProjectile.h"

// Sets default values
ATowerProjectile::ATowerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Add the root scene component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));

	//Create the projectile mesh
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	ProjectileMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Add the projectile movement component.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void ATowerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

