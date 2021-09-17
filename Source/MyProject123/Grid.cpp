// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = Root;

	LineProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Line Procedural Mesh"));
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Selection Procedural Mesh"));

	LineProceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SelectionProceduralMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
	CreateGrid();
}

void AGrid::CreateGrid()
{
	//Create the material instances for the grid lines and the hover over material
	UMaterialInstanceDynamic* LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic* SelectionMaterialInstance = CreateMaterialInstance(SelectionColor, SelectionOpacity);

	float LineStart;
	float LineEnd;

	TArray<FVector> LineVertices;
	TArray<FVector> SelectionVertices;

	TArray<int> LineTriangles;
	TArray<int> SelectionTriangles;

	//Create the horizontal lines geometry
	for (int i = 0; i <= NumRows; i++)
	{
		LineStart = (i * TileSize);
		LineEnd = GridWidth();

		CreateLine(FVector(LineStart, 0, 0), FVector(LineStart, LineEnd, 0), LineThickness, LineVertices, LineTriangles);
	}

	//Create the vertical lines geometry
	for (int i = 0; i <= NumColumns; i++)
	{
		LineStart = (i * TileSize);
		LineEnd = GridHeight();

		CreateLine(FVector(0, LineStart, 0), FVector(LineEnd, LineStart, 0), LineThickness, LineVertices, LineTriangles);
	}

	//These will not be used
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	//Create the procedural mesh for the lines
	LineProceduralMesh->CreateMeshSection_LinearColor(0, LineVertices, LineTriangles, Normals, UV0, VertexColors, Tangents, false);
	LineProceduralMesh->SetMaterial(0, LineMaterialInstance);

	//Create selection geometry and procedural mesh
	CreateLine(FVector(0, (TileSize/2), 0), FVector(TileSize ,(TileSize/2),0), TileSize, SelectionVertices, SelectionTriangles);
	//SelectionProceduralMesh->SetVisibility(false, false); TODO remove comment after testing
	SelectionProceduralMesh->CreateMeshSection_LinearColor(0, SelectionVertices, SelectionTriangles, Normals, UV0, VertexColors, Tangents, false);
	SelectionProceduralMesh->SetMaterial(0, SelectionMaterialInstance);
}

void AGrid::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int> &Triangles)
{
	float HalfThickness = (Thickness /2);
	FVector ThicknessDirection = FVector::CrossProduct((End - Start).GetSafeNormal(0.0001), FVector(0,0,1));

	//Create the vertices for the square
	Triangles.Add((Vertices.Num()+2));
	Triangles.Add((Vertices.Num()+1));
	Triangles.Add((Vertices.Num()+0));

	Triangles.Add((Vertices.Num()+2));
	Triangles.Add((Vertices.Num()+3));
	Triangles.Add((Vertices.Num()+1));

	//Set vertex locations
	Vertices.Add(Start + (ThicknessDirection * HalfThickness));
	Vertices.Add(End + (ThicknessDirection * HalfThickness));
	Vertices.Add(Start - (ThicknessDirection * HalfThickness));
	Vertices.Add(End - (ThicknessDirection * HalfThickness));

}

float AGrid::GridWidth() const
{
	float Width = (NumColumns * TileSize);
	return Width;
}

float AGrid::GridHeight() const
{
	float Height = (NumRows * TileSize);
	return Height;
}

UMaterialInstanceDynamic *AGrid::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	UMaterialInstanceDynamic* TempMaterialInstance;
	TempMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);

	TempMaterialInstance->SetVectorParameterValue("Color", Color);
	TempMaterialInstance->SetScalarParameterValue("Opacity", Opacity);

	return TempMaterialInstance;
}

bool AGrid::LocationToTile(FVector Location, int &Row, int &Column)
{
	return true;
}

bool AGrid::TileToGridLocation(int Row, int Column, bool Center, FVector2D &GridLocation)
{
	return true;
}

void AGrid::SetSelectedTile(int Row, int Column)
{

}

bool AGrid::TileIsValid(int Row, int Column)
{
	return true;
}
