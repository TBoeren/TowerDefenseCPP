// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Get the stats for this unit
	static const FString ContextString(TEXT("Tower Data"));
	FUnitStats *UnitStats = UnitData->FindRow<FUnitStats>(FName(RowName), ContextString, true);
	if (UnitStats)
	{
		UnitHealth = UnitStats->UnitHealth;
		UnitMoveSpeed = UnitStats->UnitMoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = UnitMoveSpeed;

		UE_LOG(LogTemp, Warning, TEXT("%s"), *UnitStats->UnitName);
	}
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	//Remove damage from the unit health
	UnitHealth -= DamageAmount;

	//Update the units healthbar with the new health
	ABaseEnemy::UpdateHealthBar(UnitHealth);

	//if the HP is 0 or lower,
	if (UnitHealth <= 0)
	{	
		ABaseEnemy::Death(true);
	}

	return DamageAmount;
}

void ABaseEnemy::UpdateHealthBar_Implementation(float UpdatedHealth)
{
	//Done in BP
}

void ABaseEnemy::Death(bool RewardResources)
{
	//get the resources gained upon death
	static const FString ContextString(TEXT("Tower Data"));
	FUnitStats *UnitStats = UnitData->FindRow<FUnitStats>(FName(RowName), ContextString, true);

	II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
	if (GameStateInterface && RewardResources)
	{
		//Add it to the current amount, remove from total units in wave and then destroy self
		GameStateInterface->SetResources(UnitStats->ResourcesGained);
		GameStateInterface->SetTotalUnitsInWave(-1);
		Destroy();
	}
	else
	{
		//Remove from total units
		GameStateInterface->SetTotalUnitsInWave(-1);
		Destroy();
	}
}
