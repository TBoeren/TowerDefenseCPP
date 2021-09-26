// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"
#include "Kismet/GameplayStatics.h"

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

	II_BaseGameState *GameStateInterface = Cast<II_BaseGameState>(GetWorld()->GetGameState());
	if (GameStateInterface)
	{
		GameStateInterface->SetGrid(this);
	}

	APC_Base *PlayerController = Cast<APC_Base>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController->IsValidLowLevel())
	{
		PlayerController->OnTileSelected.AddDynamic(this, &AGrid::WhenSelectedTileUpdated);
		PlayerController->OnTileUnselected.AddDynamic(this, &AGrid::WhenSelectedTileUpdated);
	}
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrid::OnConstruction(const FTransform &Transform)
{
	CreateGrid();
}

void AGrid::CreateGrid()
{
	//Create the material instances for the grid lines and the hover over material
	UMaterialInstanceDynamic *LineMaterialInstance = CreateMaterialInstance(LineColor, LineOpacity);
	UMaterialInstanceDynamic *SelectionMaterialInstance = CreateMaterialInstance(SelectionColor, SelectionOpacity);

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
	CreateLine(FVector(0, (TileSize / 2), 0), FVector(TileSize, (TileSize / 2), 0), TileSize, SelectionVertices, SelectionTriangles);
	SelectionProceduralMesh->SetVisibility(false, false);
	SelectionProceduralMesh->CreateMeshSection_LinearColor(0, SelectionVertices, SelectionTriangles, Normals, UV0, VertexColors, Tangents, false);
	SelectionProceduralMesh->SetMaterial(0, SelectionMaterialInstance);
}

void AGrid::CreateLine(FVector Start, FVector End, float Thickness, TArray<FVector> &Vertices, TArray<int> &Triangles)
{
	float HalfThickness = (Thickness / 2);
	FVector ThicknessDirection = FVector::CrossProduct((End - Start).GetSafeNormal(0.0001), FVector(0, 0, 1));

	//Create the vertices for the square
	Triangles.Add((Vertices.Num() + 2));
	Triangles.Add((Vertices.Num() + 1));
	Triangles.Add((Vertices.Num() + 0));

	Triangles.Add((Vertices.Num() + 2));
	Triangles.Add((Vertices.Num() + 3));
	Triangles.Add((Vertices.Num() + 1));

	//Set vertex locations
	Vertices.Add(Start + (ThicknessDirection * HalfThickness));
	Vertices.Add(End + (ThicknessDirection * HalfThickness));
	Vertices.Add(Start - (ThicknessDirection * HalfThickness));
	Vertices.Add(End - (ThicknessDirection * HalfThickness));
}

float AGrid::GridWidth() const
{
	//Calculate and return the width of the grid
	float Width = (NumColumns * TileSize);
	return Width;
}

float AGrid::GridHeight() const
{
	//Calculate and return the height of the grid
	float Height = (NumRows * TileSize);
	return Height;
}

UMaterialInstanceDynamic *AGrid::CreateMaterialInstance(FLinearColor Color, float Opacity)
{
	//Create the dynamic material instance
	UMaterialInstanceDynamic *TempMaterialInstance;
	TempMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);

	//Set the color and opacity and return the dynamic material instance
	TempMaterialInstance->SetVectorParameterValue("Color", Color);
	TempMaterialInstance->SetScalarParameterValue("Opacity", Opacity);

	return TempMaterialInstance;
}

bool AGrid::LocationToTile(FVector Location, int &Row, int &Column)
{
	//Convert the given position to a row and column
	int LocationRow = FMath::Floor((((Location.X - GetActorLocation().X) / GridHeight()) * NumRows));
	int LocationColumn = FMath::Floor((((Location.Y - GetActorLocation().Y) / GridWidth()) * NumColumns));

	//return the rows and check if they are valid
	Row = LocationRow;
	Column = LocationColumn;

	return TileIsValid(LocationRow, LocationColumn);
}

bool AGrid::TileToGridLocation(int Row, int Column, bool Center, FVector2D &GridLocation)
{
	//If you want the center of the tile, half of the tile size will be added to the calculations. Otherwise, you get the bottom left of the tile
	if (Center)
	{
		GridLocation.X = (((Row * TileSize) + GetActorLocation().X) + (TileSize / 2));
		GridLocation.Y = (((Column * TileSize) + GetActorLocation().Y) + (TileSize / 2));
	}
	else
	{
		GridLocation.X = ((Row * TileSize) + GetActorLocation().X);
		GridLocation.Y = ((Column * TileSize) + GetActorLocation().Y);
	}

	return TileIsValid(Row, Column);
}

void AGrid::SetSelectedTile(int Row, int Column)
{
	//Create a variable to set the grid location to
	FVector2D GridLocation;

	if (TileToGridLocation(Row, Column, false, GridLocation))
	{
		//If over a valid tile, set the visibility to true
		SelectionProceduralMesh->SetVisibility(true, false);
		SelectionProceduralMesh->SetWorldLocation(FVector(GridLocation.X, GridLocation.Y, GetActorLocation().Z), false, nullptr, ETeleportType::None);
	}
	else
	{
		//If over a invalid tile, set the visibility to false
		SelectionProceduralMesh->SetVisibility(false, false);
	}
}

bool AGrid::TileIsValid(int Row, int Column)
{
	if ((Row >= 0 && Row < NumRows) && (Column >= 0 && Column < NumColumns) && !AllOccupiedTiles.Contains(FIntPoint(Row, Column)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AGrid::WhenSelectedTileUpdated(FIntPoint Tile)
{
	CurrentlySelectedTile = Tile;
}

void AGrid::ConstructTower(TSubclassOf<ATowerBase> TowerToConstruct)
{
	FVector2D GridLocation;

	//Get the location of the current tile and check if the tile is valid
	if (TileToGridLocation(CurrentlySelectedTile.X, CurrentlySelectedTile.Y, true, GridLocation))
	{
		//When the tile has been checked, spawn the tower
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ATowerBase *TowerTemp = GetWorld()->SpawnActor<ATowerBase>(TowerToConstruct, FVector(GridLocation.X, GridLocation.Y, GetActorLocation().Z), Rotation, SpawnInfo);

		//After construction, add the tile to the currently occupied tiles and unselect the current tile.
		AddOccupiedTile(CurrentlySelectedTile);
	}
}

void AGrid::AddOccupiedTile(FIntPoint Tile)
{
	AllOccupiedTiles.Add(Tile);
}
