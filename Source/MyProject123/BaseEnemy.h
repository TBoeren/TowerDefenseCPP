// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "I_Enemy.h"
#include "I_BaseGameState.h"
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
	float UnitFlatArmor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnitPercentArmor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UnitMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ResourcesGained;
};

UCLASS()
class MYPROJECT123_API ABaseEnemy : public ACharacter, public II_Enemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UPROPERTY(EditAnywhere)
	class UDataTable* UnitData;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void Death(bool RewardResources) override;

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealthBar(float UpdatedHealth);	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString RowName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Enemy|Unit Stats")
	float UnitHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Enemy|Unit Stats")
	float UnitFlatArmor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Enemy|Unit Stats")
	float UnitPercentArmor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Enemy|Unit Stats")
	float UnitMoveSpeed;


};
