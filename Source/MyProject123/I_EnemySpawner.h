// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_EnemySpawner.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_EnemySpawner : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_EnemySpawner
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //Get the amount of waves within the spawner
    UFUNCTION()
    virtual int GetTotalWavesInSpawner() = 0;

    //Start the next wave in the spawner
    UFUNCTION()
    virtual void StartNextWave() = 0;

    //Calculate the total amount of enemies within the wave
    UFUNCTION()
    virtual int CalculateTotalEnemies() = 0;
};
