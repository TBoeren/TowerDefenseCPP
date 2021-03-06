// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_BasePawn.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_BasePawn : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT123_API II_BasePawn
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    //Moves the camera based on the position of the mouse
    UFUNCTION()
    virtual void EdgeScrollCamera(FVector2D XYValue) = 0;

    //Updates the length of the camera boom based on the float value
    UFUNCTION()
    virtual void UpdateCameraBoomLength(float Value) = 0;
};
