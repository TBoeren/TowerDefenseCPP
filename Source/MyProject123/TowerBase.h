// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "TowerProjectile.h"
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
    UTexture2D* TowerImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TowerDamage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TowerRange;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TowerAttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TowerAOERadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1")) //TODO when final variables are set, maybe convert to calculate automatically
    float TowerDamagePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1")) //TODO when final variables are set, maybe convert to calculate automatically
    float TowerRangePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "1")) //TODO when final variables are set, maybe convert to calculate automatically
    float TowerAttackSpeedPercentage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int TowerBuildTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int TowerCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText TowerInfo;
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

    //When the towers range overlaps, start that attacks
    UFUNCTION(Category = "Tower Base|Attacking")
    virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    //When the towers range ends overlaps, stop attacking the current first entry in the array
    UFUNCTION(Category = "Tower Base|Attacking")
    virtual void OnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    //The datatable that contains all the towers and their information
    UPROPERTY(EditAnywhere, Category = "Tower Base")
    class UDataTable* TowerData;

    //The current enemies in range of the tower
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower Base|Attacking")
    TArray<AActor*> EnemiesInRange;

    //The timer responsable for calling the apply damage function
    UPROPERTY(BlueprintReadWrite, Category = "Tower Base|Attacking")
    FTimerHandle AttackTimer;

    //The timer responsable for updating the rotation of the towers weapon (if applicable)
    UPROPERTY(BlueprintReadWrite, Category = "Tower Base|Attacking")
    FTimerHandle RotationTimer;

    //The timer responsable for firing the towers projectile ammo (if applicable)
    UPROPERTY(BlueprintReadWrite, Category = "Tower Base|Attacking")
    FTimerHandle VisualTimer;

    //The speed at which the tower weapon rotates (If applicable)
    UPROPERTY(EditAnywhere, Category = "Tower Base|Settings")
    float WeaponBaseRotationSpeed;

    //The speed at which the tower projectile fires. This needs to be equal to the initial speed of the assigned projectile (If applicable)
    UPROPERTY(EditAnywhere, Category = "Tower Base|Settings")
    float ProjectileSpeed;

    //The time before the attack that the visual will appear
    UPROPERTY(EditAnywhere, Category = "Tower Base|Settings")
    float VisualTime;

    //The actor class of the tower projectile that will be used
    UPROPERTY(EditAnywhere, Category = "Tower Base|Projectile")
    TSubclassOf<ATowerProjectile> TowerProjectile;

    UPROPERTY()
    FTowerStats CurrentTowerStats;

    //Function that applies damage to the enemy
    UFUNCTION()
    virtual void ApplyDamage();

    //Function returns the weapon base that will be rotated
    UFUNCTION(BlueprintCallable)
    void UpdateTowerBaseRotation(UStaticMeshComponent* WeaponToRotate, bool Return);

    //Function to be called by the rotation timer 
    UFUNCTION(BlueprintNativeEvent)
    void StartRotationTimer(bool Return);

    //Function returns the weapon base that will be rotated
    UFUNCTION(BlueprintCallable)
    void CreateTowerAttackVisual(FVector ProjectileOrigin);

public:
    //Sphere collision that is used to represent the range of the tower
    UPROPERTY(VisibleAnywhere, Category = "Tower Base|Attacking")
    class USphereComponent* SphereCollision;

    //Mesh that is used to represent the tower
    UPROPERTY(VisibleAnywhere, Category = "Tower Base")
    class UStaticMeshComponent* TowerMesh;

    //Decal used to show the range of the tower to the player
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Base")
    class UDecalComponent* RangeDecal;

    //The mesh that will be rotated to face the enemy
    UPROPERTY(EditAnywhere, Category = "Tower Base|Attacking")
    class UStaticMeshComponent* WeaponBase;

    //The row name that is used to retrieve the tower stats from the data table
    UPROPERTY(EditAnywhere, Category = "Tower Base")
    FString RowName;

    //The range decal material
    UPROPERTY(EditAnywhere, Category = "Tower Base")
    UMaterialInterface* RangeDecalMaterial;

    //Function to be called by the visual timer 
    UFUNCTION(BlueprintNativeEvent)
    void FireTowerAttackVisual();
};