// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//creare componenti
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Rootcomponent del tile 
	SetRootComponent(SceneComponent);
	StaticMeshComponent->SetupAttachment(SceneComponent);

	Status = ETileStatus::EMPTY;
	PlayerOwner = -1;
	GridPosition = FVector2D(0, 0);

}



void ATile::SetTileStatus(const int32 TileOwner, const ETileStatus TileStatus)
{
	PlayerOwner = TileOwner;
	Status = TileStatus;
}

ETileStatus ATile::GetTileStatus()
{
	return Status;
}

int32 ATile::GetTileOwner()
{
	return PlayerOwner;
}

void ATile::SetGridPosition(const double InX, const double InY)
{
	GridPosition.Set(InX, InY);
}

FVector2D ATile::GetGridPosition()
{
	return GridPosition;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}


/* Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

