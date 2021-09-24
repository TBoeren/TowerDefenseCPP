// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
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
	float TowerRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TowerAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TowerBuildTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TowerCost;
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

	UFUNCTION(Category = "Tower Base|Attacking")
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(Category = "Tower Base|Attacking")
	virtual void OnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Tower Base")
	class UDataTable* TowerData;

	UPROPERTY(EditAnywhere, Category = "Tower Base|Attacking")
	TArray<AActor*> EnemiesInRange;

	UPROPERTY(BlueprintReadWrite, Category = "Tower Base|Attacking")
	FTimerHandle AttackTimer;

	UFUNCTION()
	void ApplyDamage();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Tower Base|Attacking")
	class USphereComponent* SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "Tower Base")
	class UStaticMeshComponent* TowerMesh;

	UPROPERTY(EditAnywhere, Category = "Tower Base")
	FString RowName;
};