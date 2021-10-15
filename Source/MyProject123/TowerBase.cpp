// Fill out your copyright notice in the Description page of Project Settings.
#include "TowerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

	//Create the attack range 
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Collision"));
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATowerBase::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ATowerBase::OnOverlapEnd);
	RootComponent = SphereCollision;

    //Create the tower mesh
    TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower Mesh"));
	TowerMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ATowerBase::BeginPlay()
{
	Super::BeginPlay();

	//Create the Decal
	FRotator Rotation(-90.0f, -90.0f, 0.0f);
	
	//Get the stats for this tower
	static const FString ContextString(TEXT("Tower Data"));
	FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(FName(RowName), ContextString, true);
	if(TowerStats)
	{
		SphereCollision->SetSphereRadius(TowerStats->TowerRange, true);
		RangeDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), RangeDecalMaterial, FVector(TowerStats->TowerRange, TowerStats->TowerRange, TowerStats->TowerRange), GetActorLocation(), Rotation, 0.0f);
	}

	//Set the decal material and hide it the decal
	RangeDecal->SetDecalMaterial(RangeDecalMaterial);
	RangeDecal->SetRelativeRotation(Rotation);
	RangeDecal->SetVisibility(false, false);
}

void ATowerBase::OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Add the unit to the enemies in range array
	EnemiesInRange.Add(OtherActor);

	//And start the timer if it is not running
	if(!GetWorldTimerManager().TimerExists(AttackTimer))
    {
		//Get the attack rate from the data table
		static const FString ContextString(TEXT("Tower Data"));
		FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(FName(RowName), ContextString, true);

        GetWorldTimerManager().SetTimer(AttackTimer, this, &ATowerBase::ApplyDamage, TowerStats->TowerAttackSpeed, true);

		//Set the move information in the timer delegate
		GetWorldTimerManager().ClearTimer(RotationTimer);
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("StartRotationTimer"), false);

		GetWorldTimerManager().SetTimer(RotationTimer, TimerDelegate, 0.01f, true);
    }
}

void ATowerBase::OnOverlapEnd(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Remove the actor that has existed the range from the array
	EnemiesInRange.Remove(OtherActor);

	//Check if the array is empty. If it is, stop attacking
	if(EnemiesInRange.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);

		//Clear a time and start a new one to reset the rotation
		GetWorldTimerManager().ClearTimer(RotationTimer);
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, FName("StartRotationTimer"), true);

		GetWorldTimerManager().SetTimer(RotationTimer, TimerDelegate, 0.01f, true);
	}
}

void ATowerBase::StartRotationTimer_Implementation(bool Return)
{
	//Done in BP
}

void ATowerBase::UpdateTowerBaseRotation(UStaticMeshComponent* WeaponToRotate, bool Return)
{
    if (Return)
    {
		WeaponToRotate->SetRelativeRotation(FMath::RInterpConstantTo(WeaponToRotate->GetRelativeRotation(), FRotator(0,0,0), GetWorld()->DeltaTimeSeconds, 125.f));

		if(WeaponToRotate->GetRelativeRotation() == FRotator(0,0,0))
		{
			GetWorldTimerManager().ClearTimer(RotationTimer);
		}
    }
    else
    {
        FVector TargetLocation = EnemiesInRange[0]->GetActorLocation();
        FVector BallistaBaseLocation = WeaponToRotate->GetComponentLocation();

        FVector Direction = TargetLocation - BallistaBaseLocation;
        GetActorTransform().InverseTransformVectorNoScale(Direction);

        FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
        TargetRotation = FRotator(0, (TargetRotation.Yaw - 90), 0);

        WeaponToRotate->SetRelativeRotation(FMath::RInterpConstantTo(WeaponToRotate->GetRelativeRotation(), TargetRotation, GetWorld()->DeltaTimeSeconds, 125.f));
    }
}

void ATowerBase::ApplyDamage()
{
	//Get the attack damage from the data table
	static const FString ContextString(TEXT("Tower Data"));
	FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(FName(RowName), ContextString, true);

	//Call the apply damage on the first entry in the array, make that the target and passing the tower damage from the data table
	UGameplayStatics::ApplyDamage(EnemiesInRange[0], TowerStats->TowerDamage, nullptr, this, nullptr);
}