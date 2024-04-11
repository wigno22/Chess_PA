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





void ARandomPlayer::GiocatoreAI(int32 Player)
{

	RilevaPezzi(Player);
	SimulaMosse();
}

 //Rilevo tutti i pezzi del computer e li metto in un array per poi simulare le mosse
TArray<APiece*> ARandomPlayer::RilevaPezzi(int32 Player)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	while (PezziAI.Num() > 0)
	{
		PezziAI.RemoveAt(0); // Rimuovi il primo elemento dall'array
	}


	for (auto& CurrTile : GameMode->GField->GetTileArray())
	{
		if (CurrTile->GetTileOwner() == Player)
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

	while (GameMode->GField->Mangiate.Num() > 0)
	{
		GameMode->GField->Mangiate.RemoveAt(0); // Rimuovi il primo elemento dall'array
	}

	while (GameMode->GField->MangiateNew.Num() > 0)
	{
		GameMode->GField->MangiateNew.RemoveAt(0); // Rimuovi il primo elemento dall'array
	}

	for (APiece* Piece : PezziAI)
	{

		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
		GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y

		ATile* CurrTile = Piece->GetTile();

		if (GameMode->GField->ColorLegalMoves(Mosselegali, CurrTile))
		{
			TrovataMossa = true;
			
		}

	}

	if (TrovataMossa)
	{
		int32 Posizione = 0;
		Posizione = GameMode->GField->Mangiate.Num();

		for (int32 j = 0; j < Posizione; j++)
		{
			int32 Peso = GameMode->GField->Mangiate[0].PesoMangiante - GameMode->GField->Mangiate[0].PesoMangiato;
			int32 PesoNew = 0;
			int32 Indice = 0;

			for (int32 i = 0; i < GameMode->GField->Mangiate.Num(); i++)
			{
				PesoNew = GameMode->GField->Mangiate[i].PesoMangiante - GameMode->GField->Mangiate[i].PesoMangiato;
				if (Peso > PesoNew)
				{
					Peso = PesoNew;
					Indice = i;
					GameMode->GField->MangiateNew.Add(GameMode->GField->Mangiate[Indice]);

				}

			};




			//GameMode->GField->Mangiate[Indice].PosPezzoMangiato->SetTileOwner(1);
			(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(1, ETileStatus::OCCUPIED);

			//controllo se effettuando la mossa del nero, qualche pedina bianca possa mangiare, se la mia mossa mi rende vulnerabile non la faccio e ne cerco un'altra, altrimenti la eseguo
			if (SimulaControMossa(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))
			{
				(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(0, ETileStatus::OCCUPIED);
				//prima di rinunciare alla mossa, guardo se mangiando ottengo comunque un vantaggio in termini di peso
				if (GameMode->GField->Mangiate[Indice].PesoMangiante - GameMode->GField->Mangiate[Indice].PesoMangiato <= 0)
				{
					GameMode->GField->DoMove(GameMode->GField->Mangiate[Indice].PosPezzoMangiante, GameMode->GField->Mangiate[Indice].PosPezzoMangiato, GameMode->GField->Mangiate[Indice].Player);
					TrovataMossa = true;
					break;
				}
				else
				{
					TrovataMossa = false;
					GameMode->GField->Mangiate.RemoveAt(Indice);
					Posizione = GameMode->GField->Mangiate.Num();
				}


			}
			else
			{
				(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(0, ETileStatus::OCCUPIED);
				GameMode->GField->DoMove(GameMode->GField->Mangiate[Indice].PosPezzoMangiante, GameMode->GField->Mangiate[Indice].PosPezzoMangiato, GameMode->GField->Mangiate[Indice].Player);
				break;
			}

		}
		 
	}

	//Ricalcolo pezzi Neri
	RilevaPezzi(1);

	if (!TrovataMossa)
	{
		for (APiece* Piece : PezziAI)
		{

			TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
			GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y
			if (Mosselegali.Num() > 0 && GameMode->CurrentPlayer == 1)
			{
				//metto in una variabile int un numero casuale tra 0 e il numero di mosse legali possibili
				int32 RandomMove = FMath::RandRange(0, Mosselegali.Num() - 1);

				GameMode->GField->DoMove(Piece->GetTile()->GetGridPosition(), Mosselegali[RandomMove], GameMode->CurrentPlayer);
				GameMode->GField->ResetLegalMoves();
				break;
			}
			
		}
		 
	}
	 
	GameMode->CurrentPlayer = 0;
	
}


bool ARandomPlayer::SimulaControMossa(FVector2D TilePosition)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	bool TrovataMossa = false;


	RilevaPezzi(0);

	for (APiece* Piece : PezziAI)
	{

		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
		
		for (int i = 0; i < Mosselegali.Num(); i++)
		{
			int32 ProprietarioTile = (*GameMode->GField->TileMap.Find(Mosselegali[i]))->GetTileOwner();

			
			if (ProprietarioTile != -1 && ProprietarioTile != 0)
			{				

				if ((*GameMode->GField->TileMap.Find(Mosselegali[i]))->GetGridPosition() == TilePosition)
				{
					return true;
				}

			}

		}

	}
	return false;
}


void ARandomPlayer::OnWin()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	//GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	//GameInstance->IncrementScoreHumanPlayer();
}
