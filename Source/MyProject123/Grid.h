// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "I_Grid.h"
#include "I_BaseGameState.h"
#include "GS_Base.h"
#include "TowerBase.h"
#include "PC_Base.h"
#include "Grid.generated.h"

UCLASS()
class MYPROJECT123_API AGrid : public AActor, public II_Grid
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	int NumRows;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	int NumColumns;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	float TileSize;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	float LineThickness;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	FLinearColor LineColor;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	FLinearColor SelectionColor;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	float LineOpacity;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	float SelectionOpacity;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	UMaterialInterface* GridMaterial;

	UPROPERTY(EditAnywhere, Category = "Grid|Constants")
	UMaterialInterface* RangeDecalMaterial;

	UPROPERTY(VisibleAnywhere, Category = "Grid|Tower Placement")
	TMap<FIntPoint, ATowerBase*> AllOccupiedTiles;

	UPROPERTY(VisibleAnywhere, Category = "Grid|Tower Placement")
	FIntPoint CurrentlySelectedTile;

	UPROPERTY()
	UProceduralMeshComponent* LineProceduralMesh;

	UPROPERTY()
	UProceduralMeshComponent* SelectionProceduralMesh;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	ADecalActor* RangeDecal;

	UFUNCTION(BlueprintCallable)
	void CreateGrid();

	UFUNCTION()
	void CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int> &Triangles);

	UFUNCTION()
	float GridWidth() const;

	UFUNCTION()
	float GridHeight() const;

	UFUNCTION()
	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor Color, float Opacity);

	UFUNCTION()
	void WhenSelectedTileUpdated(FIntPoint Tile);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool LocationToTile (FVector Location, bool BuildingSelected, int &Row, int &Column) override;
	virtual bool TileToGridLocation(int Row, int Column, bool Center, FVector2D &GridLocation) override;
	virtual void SetSelectedTile(int Row, int Column) override;
	virtual bool TileIsValid(int Row, int Column, bool BuildingSelected) override;
	virtual void AddOccupiedTile(FIntPoint Tile, ATowerBase* Tower) override;

	virtual void ConstructTower(TSubclassOf<ATowerBase> TowerToConstruct) override;
	virtual void ConstructRangeDecal(float TowerRange) override;
	virtual void DestroyRangeDecal() override;
	virtual void SellTower(AActor* TowerToSell) override;
};
