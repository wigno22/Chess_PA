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



/* Called every frame
void APiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APiece::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

