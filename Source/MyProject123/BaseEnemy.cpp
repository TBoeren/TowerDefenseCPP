// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "GS_Base.h"
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
    FUnitStats* UnitStats = UnitData->FindRow<FUnitStats>(FName(RowName), ContextString, true);
    if (UnitStats)
    {
        //Set the variables based on the stats in the datatable
        float UnscaledUnitHealth = UnitStats->UnitHealth;
        UnitMoveSpeed = UnitStats->UnitMoveSpeed;
        UnitFlatArmor = UnitStats->UnitFlatArmor;
        UnitPercentArmor = UnitStats->UnitPercentArmor;
        UnitHealth = (UnscaledUnitHealth / ((100 - UnitPercentArmor) / 100));
        GetCharacterMovement()->MaxWalkSpeed = UnitMoveSpeed;
    }
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    //Remove damage from the unit health
    UnitHealth -= FMath::Clamp((DamageAmount - UnitFlatArmor), 1.f, 10000.f);

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
    FUnitStats* UnitStats = UnitData->FindRow<FUnitStats>(FName(RowName), ContextString, true);

    //Call to the game state interface and add the gained resources to the current amount, remove from total units in wave and then destroy self
    II_BaseGameState* GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
    if (GameStateInterface && RewardResources)
    {
        GameStateInterface->SetResources(UnitStats->ResourcesGained);
        GameStateInterface->SetTotalUnitsInWave(-1);
        Destroy();
    }
    else
    {
        //Remove the destroyed unit from total units without giving the player resources
        GameStateInterface->SetTotalUnitsInWave(-1);
        Destroy();
    }
}
