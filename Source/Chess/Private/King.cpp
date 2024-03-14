// Fill out your copyright notice in the Description page of Project Settings.


#include "King.h"
//#include "Tile.h"


// Sets default values
AKing::AKing()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/KingMatW.KingMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/KingMatW.KingMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void AKing::BeginPlay()
{
	Super::BeginPlay();


	
}




/*void AKing::Move(FVector newPosition)
{
	
	// Verifica che la mossa sia valida
	if (FVector::Dist(position, newPosition) <= 1) {
		position = newPosition;
	}
	else {
		// Gestisci il caso in cui la mossa non è valida
	}
}*/



/* Called every frame
void AKing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKing::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

