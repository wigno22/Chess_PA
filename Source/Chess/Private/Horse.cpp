// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse.h"

// Sets default values
AHorse::AHorse()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/HorseMatW.HorseMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	// Supponiamo che 'MeshComponent' sia il tuo UStaticMeshComponent
	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/HorseMatW.HorseMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void AHorse::BeginPlay()
{
	Super::BeginPlay();

}
