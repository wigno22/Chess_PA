// Fill out your copyright notice in the Description page of Project Settings.
#include "Piece.h"
#include "Tile.h"
#include "ChessGameMode.h"


// Sets default values
APiece::APiece()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Size of the chessboard
	BoardSize = 8;
	//Tile size
	TileSize = 100;

	//creare componenti
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Rootcomponent del tile 
	SetRootComponent(SceneComponent);
	StaticMeshComponent->SetupAttachment(SceneComponent);
	//Tile padding
	//CellPadding = 0;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));

	if (MeshFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshFinder.Object);
	}
}

// Called when the game starts or when spawned
void APiece::BeginPlay()
{
	Super::BeginPlay();

	

}



void APiece::Move(FVector newPosition)
{
	position = newPosition;
}

int32 APiece::GetPlayerOwner()
{
	return int32();
}

void APiece::SetPlayerOwner(int32 NewOwner)
{
		PlayerOwner = NewOwner;
}

TArray<FVector2D> APiece::CalculateMoves(ATile* CurrTile)
{
	return TArray<FVector2D>();
}


FString APiece::GetTipoPedina() const
{
	return TipoPedina;
}

void APiece::SetTipoPedina(FString NewTipoPedina)
{
		TipoPedina = NewTipoPedina;
}

FString APiece::GetName() const
{
	return Name;
}

void APiece::SetName(FString NewName)
{
		Name = NewName;
}

int32 APiece::GetWeight() const
{
	return Weight;
}

void APiece::SetWeight(int32 NewWeight)
{
	Weight = NewWeight;
}

ATile* APiece::GetTile()
{
	return CurrentTile;
}

void APiece::SetCurrentTile(ATile* NewTile)
{
		CurrentTile = NewTile;
}

void APiece::SetGridPosition(const double InX, const double InY)
{
	GridPosition.Set(InX, InY);
}

FVector2D APiece::GetGridPosition()
{
	 return GridPosition; 

}

void APiece::SelfDestroy()
{
		Destroy();
}

