// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

USTRUCT(BlueprintType)
struct FUnitStats : public FTableRowBase
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABaseEnemy> EnemyBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString UnitName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnitHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnitMoveSpeed;
};

UCLASS()
class MYPROJECT123_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UDataTable* UnitData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	FString RowName;

};
