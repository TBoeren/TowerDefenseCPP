// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GS_Base.h"
#include "GameFramework/PlayerController.h"
#include "I_BaseGameState.h"
#include "I_BasePawn.h"
#include "I_BasePlayerController.h"
#include "I_Grid.h"
#include "TowerBase.h"
#include "Blueprint/UserWidget.h"
#include "PC_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTowerDelegate, FIntPoint, SelectedTile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildingDelegate, AActor*, SelectedTower);

UCLASS()
class MYPROJECT123_API APC_Base : public APlayerController, public II_BasePlayerController
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    APC_Base();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //Determines how close the mouse needs to be at the edge of the screen for it to start scrolling
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
    float EdgeScrollTolerance;

    //The speed at which the camera scrolls
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
    float EdgeScrollSpeed;

    //The speed at which the camera zooms
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
    float ZoomSpeed;

    //The speed at which time increases when the speed is updated
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Settings")
    float TimeDilationSpeed;

    //The tile that is currently selected
    UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Tower Placement")
    FIntPoint CurrentTile;

    //All the grids on the map. Retrieved from the game state
    UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Grid")
    TArray<AActor*> Grids;

    //The grid that is currently being hovered/selected
    UPROPERTY(VisibleAnywhere, Category = "Base PlayerController|Grid")
    AActor* CurrentlySelectedGrid;

    //The datatable that contains all the towers and their information
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Tower Placement")
    class UDataTable* TowerData;

    //The datatable that contains all the towers and their information
    UPROPERTY(EditAnywhere, Category = "Base PlayerController|Main Widget")
    TSubclassOf<UUserWidget> MainWidgetClass;

    //Checks if a tile or building is currently selected
    UPROPERTY()
    bool LocationSelected = false;

public:
    // Called to bind functionality to input
    virtual void SetupInputComponent() override;

    UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Placement")
    FTowerDelegate OnTileSelected;

    UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Placement")
    FTowerDelegate OnTileUnselected;

    UPROPERTY(BlueprintAssignable, Category = "Base PlayerController | Tower Selection")
    FBuildingDelegate OnTowerSelected;

private:
    //Called when the mouse moves
    UFUNCTION()
    void MouseMove(float Value);

    //Called when the select button is pressed
    UFUNCTION()
    void OnSelectButtonDown();

    //Called when the cancel button is pressed
    UFUNCTION()
    void OnCancelButtonDown();

    //Called when scrolling up
    UFUNCTION()
    void CameraZoomIn();

    //Called when scrolling down
    UFUNCTION()
    void CameraZoomOut();

    //Called when the pause button is pressed to pause the game
    UFUNCTION()
    void UpdatePause();

    //Called when the Update speed button is pressed
    UFUNCTION()
    void IncreaseGameSpeed();

    //Called when the Update speed button is released
    UFUNCTION()
    void ResetGameSpeed();

    //Checks to see if we are currently in the main menu
    UFUNCTION()
    bool IsInMainMenu();

    //Checks if the mouse is close enough near the edge for edge scrolling
    UFUNCTION()
    FVector2D CalculateEdgeScroll(FVector2D MousePosition);

    //Interface Functions
    void PassTowerToConstruct_Implementation(FName TowerRowName) override;
    void PassTowerRange_Implementation(FName TowerRowName) override;
    void PassTowerSell_Implementation(FName TowerRowName, AActor* TowerToSell) override;
};
