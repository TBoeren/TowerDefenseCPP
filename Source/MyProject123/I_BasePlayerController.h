// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_BasePlayerController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_BasePlayerController : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_BasePlayerController
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //A blueprint callable funtion that passes row name of the tower to construct
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    void PassTowerToConstruct(FName TowerRowName);

    //A blueprint callable funtion that passes row name of the tower to retrieve the range from
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    void PassTowerRange(FName TowerRowName);

    //A blueprint callable funtion that passes row name of the tower to retrieve cost from and passes the tower actor to be sold
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Tower Placement")
    void PassTowerSell(FName TowerRowName, AActor* TowerToSell);
};
