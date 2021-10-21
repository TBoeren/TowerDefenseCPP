// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "I_BaseGameState.h"
#include "I_Enemy.h"
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
    UTexture2D* UnitImage;

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

    void CalculateScaledHealth(){UnitHealth = (UnitHealth / ((100 - UnitPercentArmor) / 100));};
    void UpdateHealthOnAttack(float Value){UnitHealth -= FMath::Clamp((Value - UnitFlatArmor), 1.f, 10000.f);};
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

    //Called when damage is taken
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    //This data table is used to retrieve the unit stats from
    UPROPERTY(EditAnywhere, Category = "Base Enemy|Unit Stats")
    class UDataTable* UnitData;

public:
    UFUNCTION()
    virtual void Death(bool RewardResources) override;

    UFUNCTION(BlueprintNativeEvent)
    void UpdateHealthBar(float UpdatedHealth);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Enemy|Unit Stats")
    FString RowName;
private:
    UPROPERTY()
    FUnitStats CurrentUnitStats;
};
