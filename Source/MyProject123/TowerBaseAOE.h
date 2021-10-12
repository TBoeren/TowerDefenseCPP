// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerBase.h"
#include "TowerBaseAOE.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT123_API ATowerBaseAOE : public ATowerBase
{
    GENERATED_BODY()

    virtual void ApplyDamage() override;

public:
    UPROPERTY(EditAnywhere, Category = "AOE Tower|Settings")
    bool AttackOriginSelf;
};
