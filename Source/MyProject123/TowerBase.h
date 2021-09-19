// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/SphereComponent.h"
#include "TowerBase.generated.h"

USTRUCT(BlueprintType)
struct FTowerStats : public FTableRowBase
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATowerBase> TowerBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString TowerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TowerDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TowerAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TowerBuildTime;
};

UCLASS()
class MYPROJECT123_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UDataTable* TowerData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TowerMesh;

	UPROPERTY(EditAnywhere)
	FString RowName;
};