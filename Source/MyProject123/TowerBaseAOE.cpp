// Fill out your copyright notice in the Description page of Project Settings.

#include "TowerBaseAOE.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

void ATowerBaseAOE::ApplyDamage()
{
    //if true, create the radius around yourself instead of firing a projectile
    if (AttackOriginSelf)
    {
        //Call the apply radial damage on the first entry in the array, make that the target and passing the tower damage from the data table
        TArray<AActor*> ActorsToIgnore;
        UGameplayStatics::ApplyRadialDamage(GetWorld(), CurrentTowerStats.TowerDamage, GetActorLocation(), CurrentTowerStats.TowerAOERadius, nullptr, ActorsToIgnore, this, nullptr, true, ECollisionChannel::ECC_Visibility);

        //Check if the array is not empty. If it is, stop creating the visual
        if (!EnemiesInRange.Num() == 0)
        {
            //Start the timer for the projectile visual
            GetWorldTimerManager().SetTimer(VisualTimer, this, &ATowerBase::FireTowerAttackVisual, (CurrentTowerStats.TowerAttackSpeed - VisualTime), false);
        }
    }
    else
    {

        //Call the apply radial damage on the first entry in the array, make that the target and passing the tower damage from the data table
        TArray<AActor*> ActorsToIgnore;
        UGameplayStatics::ApplyRadialDamage(GetWorld(), CurrentTowerStats.TowerDamage, EnemiesInRange[0]->GetActorLocation(), CurrentTowerStats.TowerAOERadius, nullptr, ActorsToIgnore, this, nullptr, true, ECollisionChannel::ECC_Visibility);

        //Check if the array is not empty. If it is, stop creating the visual
        if (!EnemiesInRange.Num() == 0)
        {
            //Start the timer for the projectile visual
            GetWorldTimerManager().SetTimer(VisualTimer, this, &ATowerBase::FireTowerAttackVisual, (CurrentTowerStats.TowerAttackSpeed - VisualTime), false);
        }
    }
}