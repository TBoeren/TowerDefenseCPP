// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_Grid.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_Grid : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_Grid
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //Convert the given location to tile coordinates
    UFUNCTION()
    virtual bool LocationToTile(FVector Location, bool BuildingSelected, int& Row, int& Column) = 0;

    //Convert the tile coordinates to a location
    UFUNCTION()
    virtual bool TileToGridLocation(int Row, int Column, bool Center, FVector2D& GridLocation) = 0;

    //Set the currently hovered/selected tile
    UFUNCTION()
    virtual void SetSelectedTile(int Row, int Column) = 0;

    //Check if the current location is valid on the grid
    UFUNCTION()
    virtual bool TileIsValid(int Row, int Column, bool BuildingSelected) = 0;

    //Add the selected tile to the occupied tile TMap
    UFUNCTION()
    virtual void AddOccupiedTile(FIntPoint Tile, ATowerBase* Tower) = 0;

    //Construct the given tower
    UFUNCTION()
    virtual void ConstructTower(TSubclassOf<ATowerBase> TowerToConstruct) = 0;

    //Construct the range decal based on the passed float
    UFUNCTION()
    virtual void ConstructRangeDecal(float TowerRange) = 0;

    //Destroy the ranged decal
    UFUNCTION()
    virtual void DestroyRangeDecal() = 0;

    //Sell the selected tower
    UFUNCTION()
    virtual void SellTower(AActor* TowerToSell) = 0;
};