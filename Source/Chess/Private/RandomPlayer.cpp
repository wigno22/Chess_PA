// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include <Piece.h>
#include <HumanPlayer.h>

// Sets default values
ARandomPlayer::ARandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARandomPlayer::OnTurn()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	UE_LOG(LogTemp, Warning, TEXT("Random Player Turn"));


	TArray <APiece*> BlackPiece;

	//riempo l'array delle pedine nere scorrendo la tilemap e sulle tile con owner 1 aggiungo la pedina

	GiocatoreAI();
   

	GameMode->bIsMyTurn = true;
}



void ARandomPlayer::GiocatoreAI()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	RilevaPezzi();
	SimulaMosse();
}

void ARandomPlayer::RilevaPezzi()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	for (auto& CurrTile : GameMode->GField->GetTileArray())
	{
		if (CurrTile->GetTileOwner() == 1)
		{
			PezziAI.Add(CurrTile->GetPiece());
		}
	}

}

void ARandomPlayer::SimulaMosse()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	for (APiece* Piece : PezziAI)
	{

		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
		GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y
		GameMode->GField->ColorLegalMoves(Mosselegali);

	}
	
}