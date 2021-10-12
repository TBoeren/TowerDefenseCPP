// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBaseAOE.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void ATowerBaseAOE::ApplyDamage()
{
    //Get the attack damage from the data table
    static const FString ContextString(TEXT("Tower Data"));
    FTowerStats* TowerStats = TowerData->FindRow<FTowerStats>(FName(RowName), ContextString, true);

    //if true, create the radius around yourself instead of firing a projectile
    if (AttackOriginSelf)
    {
        //Call the apply radial damage on the first entry in the array, make that the target and passing the tower damage from the data table
        TArray<AActor*> ActorsToIgnore;
        UGameplayStatics::ApplyRadialDamage(GetWorld(), TowerStats->TowerDamage, GetActorLocation(), TowerStats->TowerAOERadius, nullptr, ActorsToIgnore, this, nullptr, true, ECollisionChannel::ECC_Visibility);

        //Create a visual for the AOE
        DrawDebugSphere(GetWorld(), GetActorLocation(), TowerStats->TowerAOERadius, 10, FColor::Black, false, 5.f);
    }
    else
    {

        //Call the apply radial damage on the first entry in the array, make that the target and passing the tower damage from the data table
        TArray<AActor*> ActorsToIgnore;
        UGameplayStatics::ApplyRadialDamage(GetWorld(), TowerStats->TowerDamage, EnemiesInRange[0]->GetActorLocation(), TowerStats->TowerAOERadius, nullptr, ActorsToIgnore, this, nullptr, true, ECollisionChannel::ECC_Visibility);

        //Create a visual for the AOE
        DrawDebugSphere(GetWorld(), EnemiesInRange[0]->GetActorLocation(), TowerStats->TowerAOERadius, 10, FColor::Black, false, 5.f);
    }
}