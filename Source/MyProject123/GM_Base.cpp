// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Base.h"
#include "GS_Base.h"
#include "PS_Base.h"
#include "PC_Base.h"
#include "PA_Base.h"
#include "HUD_Base.h"

AGM_Base::AGM_Base()
{
    GameStateClass = AGS_Base::StaticClass();
    PlayerStateClass = APS_Base::StaticClass();
    PlayerControllerClass = APC_Base::StaticClass();
    DefaultPawnClass = APA_Base::StaticClass();
    HUDClass = AHUD_Base::StaticClass();
}