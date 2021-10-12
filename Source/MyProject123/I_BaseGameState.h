// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_BaseGameState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_BaseGameState : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_BaseGameState
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //Adds the grid to the grid array in the game state
    UFUNCTION()
    virtual void SetGrid(AActor* Grid) = 0;

    //Get the array of grids from the game state
    UFUNCTION()
    virtual void GetGrids(TArray<AActor*>& Grids) = 0;

    //Sets the enemy goal variable in the game state
    UFUNCTION()
    virtual void SetEnemyGoal(FVector EnemyGoal) = 0;

    //Get the enemy goal from the game state
    UFUNCTION()
    virtual void GetEnemyGoal(FVector& EnemyGoal) = 0;

    //Set the amount of lives in the game state
    UFUNCTION()
    virtual void SetLives(int Lives) = 0;

    //Set the amount of building resources in the game state
    UFUNCTION()
    virtual void SetResources(int Resources) = 0;

    //Get the current amount of lives from the game state
    UFUNCTION()
    virtual int GetLivesPure() = 0;

    //Get the current amount of building resources from the game state
    UFUNCTION()
    virtual int GetResourcesPure() = 0;

    //A blueprint callable function to get the current amount of lives from the game state
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    int GetLives();

    //A blueprint callable function to get the current amount of building resources from the game state
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    int GetResources();

    //A blueprint callable function to get the towers that are currently allowed on the map
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    TArray<FName> GetAvailableTowers();

    //Set the current wave variable in the game state
    UFUNCTION()
    virtual void SetCurrentWave(int Wave) = 0;

    //Set the total units in wave variable in the game state
    UFUNCTION()
    virtual void SetTotalUnitsInWave(int TotalUnits) = 0;

    //Get the total units in wave variable from the game state
    UFUNCTION()
    virtual int GetTotalWavesPure() = 0;

    //Get the total amount of waves from the game state
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Wave Management")
    int GetTotalWaves();

    //Start the countdown of the next wave in the game state
    UFUNCTION()
    virtual void StartNextWaveCountdown(int Seconds) = 0;

    //Inform the game state that the first tower is placed
    UFUNCTION()
    virtual bool FirstTowerPlaced() = 0;

    //A blueprint callable function that tells the game state to pause the game
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game Management")
    void PauseGame(bool Pause);
};
