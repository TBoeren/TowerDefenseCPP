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

	UFUNCTION()
	virtual void SetGrid(AActor* Grid) = 0;

	UFUNCTION()
	virtual void GetGrid(AActor* &Grid) = 0;
};
