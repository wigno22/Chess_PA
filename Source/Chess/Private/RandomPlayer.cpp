// Fill out your copyright notice in the Description page of Project Settings.

#include "RandomPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include <Piece.h>
#include <HumanPlayer.h>
#include <GameMapsSettings.h>
#include <AI/NavigationSystemBase.h>





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





void ARandomPlayer::GiocatoreAI()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	RilevaPezzi();
	SimulaMosse();
}

 //Rilevo tutti i pezzi del computer e li metto in un array per poi simulare le mosse
TArray<APiece*> ARandomPlayer::RilevaPezzi()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	while (PezziAI.Num() > 0)
	{
		PezziAI.RemoveAt(0); // Rimuovi il primo elemento dall'array
	}


	for (auto& CurrTile : GameMode->GField->GetTileArray())
	{
		if (CurrTile->GetTileOwner() == 1)
		{
			PezziAI.Add(CurrTile->GetPiece());
		}
	}

	return PezziAI;
}

void ARandomPlayer::SimulaMosse()
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	bool TrovataMossa = false;

	for (APiece* Piece : PezziAI)
	{

		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
		GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y

		ATile* CurrTile = Piece->GetTile();

		if (GameMode->GField->ColorLegalMoves(Mosselegali, CurrTile))
		{
			TrovataMossa = true;
			GameMode->CurrentPlayer = 0;
			break;
		}

	}

	if (!TrovataMossa)
	{
		for (APiece* Piece : PezziAI)
		{
			APiece* PieceToMove = Piece;
			TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
			GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y
			if (Mosselegali.Num() > 0 && GameMode->CurrentPlayer == 1)
			{
				//prima di fare la mossa devo scegliere il pezzo da muovere in base a quello che ha il peso minore
				if (Piece->GetWeight() < PieceToMove->GetWeight())
				{
					PieceToMove = Piece;
				}
					
				GameMode->GField->DoMove(PieceToMove->GetTile()->GetGridPosition(), Mosselegali[0], GameMode->CurrentPlayer);
				GameMode->GField->ResetLegalMoves();
				break;
			}
			
		}
		GameMode->CurrentPlayer = 0;
		 
	}
	 
	GameMode->CurrentPlayer = 0;
	
}