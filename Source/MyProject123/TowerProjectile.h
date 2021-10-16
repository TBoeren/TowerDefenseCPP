// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TowerProjectile.generated.h"

UCLASS()
class MYPROJECT123_API ATowerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//Mesh that is used to represent the tower projectile
    UPROPERTY(VisibleAnywhere, Category = "Tower Projectile")
    class UStaticMeshComponent* ProjectileMesh;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = "Tower Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
};
