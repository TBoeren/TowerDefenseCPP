// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Grid.generated.h"

UCLASS()
class MYPROJECT123_API AGrid : public AActor
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

	UProceduralMeshComponent* LineProceduralMesh;
	UProceduralMeshComponent* SelectionProceduralMesh;
	USceneComponent* Root;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool LocationToTile (FVector Location, int &Row, int &Column);

	UFUNCTION()
	bool TileToGridLocation(int Row, int Column, bool Center, FVector2D &GridLocation);

	UFUNCTION()
	void SetSelectedTile(int Row, int Column);

	UFUNCTION()
	bool TileIsValid(int Row, int Column);

	virtual void OnConstruction(const FTransform& Transform) override;

};
