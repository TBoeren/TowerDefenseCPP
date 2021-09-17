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
	UFUNCTION()
	virtual bool LocationToTile (FVector Location, int &Row, int &Column) = 0;

	UFUNCTION()
	virtual bool TileToGridLocation(int Row, int Column, bool Center, FVector2D &GridLocation) = 0;

	UFUNCTION()
	virtual void SetSelectedTile(int Row, int Column) = 0;

	UFUNCTION()
	virtual bool TileIsValid(int Row, int Column) = 0;
};
