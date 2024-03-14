// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPed.h"

// Sets default values
APawnPed::APawnPed()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/PawnMatW.PawnMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	// Supponiamo che 'MeshComponent' sia il tuo UStaticMeshComponent
	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/PawnMatW.PawnMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void APawnPed::BeginPlay()
{
	Super::BeginPlay();

}
