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

	// get the game instance reference
	GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


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

void ARandomPlayer::GiocatoreAI(int32 Player, FVector2D Mossa)
{

	RilevaPezzi(Player);
	SimulaMosse(Mossa);
	
}

 //Rilevo tutti i pezzi del computer e li metto in un array per poi simulare le mosse
TArray<APiece*> ARandomPlayer::RilevaPezzi(int32 Player)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	while (PezziAI.Num() > 0)
	{
		PezziAI.RemoveAt(0); // Rimuovi il primo elemento dall'array
	}


	for (auto& CurrTile : GameMode->GField->TileArray)
	{
		if (CurrTile->GetTileOwner() == Player )
		{
			PezziAI.Add(CurrTile->GetPiece());
		}
		
	}

	return PezziAI;
}

void ARandomPlayer::SimulaMosse(FVector2D MossaRe)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	bool TrovataMossa = false;


	if (MossaRe.X >= 0 && GameMode->CurrentPlayer == 1)
	{
		//Sposto il re che è sotto attacco 
		GameMode->GField->DoMove(GameMode->GField->TrovaRe(GameMode->CurrentPlayer), MossaRe, GameMode->CurrentPlayer);
		GameMode->GField->ResetLegalMoves();
		return;
	}



	while (GameMode->GField->Mangiate.size() > 0)
	{
		GameMode->GField->Mangiate.erase(GameMode->GField->Mangiate.begin() + 0); // Rimuovi il primo elemento dall'array
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


	FVector2D MossaINI = FVector2D(-1, -1);
	FVector2D MossaEND = FVector2D(-1, -1);
	int UserOwner = -1;




	if (TrovataMossa)
	{
		int32 Posizione = 0;
		Posizione = GameMode->GField->Mangiate.size();


		// Ordinamento dell'array utilizzando l'algoritmo di ordinamento rapido
		GameMode->GField->quickSort(0, GameMode->GField->Mangiate.size() - 1);



		//ci serve scorrere tutte le possibili perchè simulando la contromossa potrebbe non convenire la prima
		for (int32 Indice = 0; Indice < Posizione; Indice++)
		{

			//GameMode->GField->Mangiate[Indice].PosPezzoMangiato->SetTileOwner(1);
			(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(1, ETileStatus::OCCUPIED);

			//controllo se effettuando la mossa del nero, qualche pedina bianca possa mangiare, se la mia mossa mi rende vulnerabile non la faccio e ne cerco un'altra, altrimenti la eseguo
			if (SimulaControMossa(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))
			{
				(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(0, ETileStatus::OCCUPIED);
				//prima di rinunciare alla mossa, guardo se mangiando ottengo comunque un vantaggio in termini di peso
				if (GameMode->GField->Mangiate[Indice].PesoMangiante - GameMode->GField->Mangiate[Indice].PesoMangiato <= 0)
				{
					MossaINI = GameMode->GField->Mangiate[Indice].PosPezzoMangiante;
					MossaEND = GameMode->GField->Mangiate[Indice].PosPezzoMangiato;
					UserOwner = GameMode->GField->Mangiate[Indice].Player;
					GameMode->GField->DoMove(MossaINI, MossaEND, UserOwner);
					TrovataMossa = true;
					break;
				}
				else
				{
					TrovataMossa = false;
					GameMode->GField->Mangiate.erase(GameMode->GField->Mangiate.begin() + 0);
					Posizione = GameMode->GField->Mangiate.size();
					Indice--;
				}


			}
			else
			{
				(*GameMode->GField->TileMap.Find(GameMode->GField->Mangiate[Indice].PosPezzoMangiato))->SetTileStatus(0, ETileStatus::OCCUPIED);
				//	GameMode->GField->DoMove(GameMode->GField->Mangiate[Indice].PosPezzoMangiante, GameMode->GField->Mangiate[Indice].PosPezzoMangiato, GameMode->GField->Mangiate[Indice].Player);

				MossaINI = GameMode->GField->Mangiate[Indice].PosPezzoMangiante;
				MossaEND = GameMode->GField->Mangiate[Indice].PosPezzoMangiato;
				UserOwner = GameMode->GField->Mangiate[Indice].Player;
				GameMode->GField->DoMove(MossaINI, MossaEND, UserOwner);


				break;
			}

		}

	}



	if (!TrovataMossa)
	{

		TArray<FVector2D> Mosselegali;
		AChessboard::Spostato Mossa{};
		RilevaPezzi(GameMode->CurrentPlayer);

		MossaRandom.clear();

		for (APiece* Piece : PezziAI)
		{

			Mosselegali = Piece->CalculateMoves(Piece->GetTile());
			GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y

			//mossa di partenza del pezzo
			Mossa.PosInit = Piece->GetTile()->GetGridPosition();

			//Faccio un for sulle mosselegali
			for (int i = 0; i < Mosselegali.Num(); i++)
			{

				Mossa.PosFin = Mosselegali[i];

				MossaRandom.push_back(Mossa);
			}


		}
		if (MossaRandom.size() > 0 && GameMode->CurrentPlayer == 1)
		{
			//metto in una variabile int un numero casuale tra 0 e il numero di mosse legali possibili
			int32 RandomMove = FMath::RandRange(0, MossaRandom.size() - 1);

			//	GameMode->GField->DoMove(MossaRandom[RandomMove].PosInit, MossaRandom[RandomMove].PosFin, GameMode->CurrentPlayer);

			MossaINI = MossaRandom[RandomMove].PosInit;
			MossaEND = MossaRandom[RandomMove].PosFin;
			UserOwner = GameMode->CurrentPlayer;
			GameMode->GField->DoMove(MossaINI, MossaEND, UserOwner);


			GameMode->GField->ResetLegalMoves();

		}

	}



	/////////////////////////////////////////////////
	//////////////
	//////////////
	////////////////////////////////////////////////

	///controllo che con la mossa nera ho il re in scacco o pure no

	int RisultatoScacco = ControlloScaccoNum(UserOwner);

	if (RisultatoScacco == 2)
	{
		//se il re è in scacco torno indietro sulla mossa nera 
		GameMode->GField->DoMove(MossaEND, MossaINI, UserOwner);

		GameInstance->SetTurnMessage(TEXT("Scacco !"));

		//Calcolo le mosse legali del re
		TArray<FVector2D> MosScacco = ControlloScacco(UserOwner);

		MossaINI = GameMode->GField->TrovaRe(UserOwner);
		MossaEND = MosScacco[0];
		UserOwner = GameMode->CurrentPlayer;

		//Sposto il re e lo salvo in una variabile
		GameMode->GField->DoMove(MossaINI, MossaEND, UserOwner);


	}
	if (RisultatoScacco == 3)
	{
		GameInstance->SetTurnMessage(TEXT("Scacco Matto!"));
		GameMode->bIsMyTurn = false;
		GameMode->Winner = 0;  //rendere dinamico
		GameMode->OnWin();
	}
	else
	{
		 
	}


	GameMode->CurrentPlayer = 0;
	
}

TArray<FVector2D>  ARandomPlayer::SimulaMosseControlloScacco(FVector2D TilePosition, int32 GiocatoreAVV)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	 

	FVector2D Mossa = FVector2D(-1, -1);
	int32 Indice = 0;
	TArray<FVector2D> MosselegaliAvv; 
	  
	while (GameMode->GField->Mangiate.size() > 0)
	{
		GameMode->GField->Mangiate.erase(GameMode->GField->Mangiate.begin() + 0); // Rimuovi il primo elemento dall'array
	}

	APiece* Piece = nullptr;
	//Trovo il pezzo che voglio controllare se ha mosse legali
	Piece = GameMode->GField->GetTileAtPosition(TilePosition)->GetPiece();
	//Trovo tutte le mosse legali del pezzo
	TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());

	if(Mosselegali.IsEmpty())
	{
		return Mosselegali;
	} 
	 
	// Trovo i pezzi avverssario 
	RilevaPezzi(GiocatoreAVV);


	for (APiece* PieceNew : PezziAI)
	{
		for (FVector2D MossalegAvv : PieceNew->CalculateMoves(PieceNew->GetTile()))
		{
			MosselegaliAvv.Add(MossalegAvv);
		}
	}		
		
		
	//se il pezzo è sotto scacco, non ha mosse legali e non può muoversi

			
	for (int j = 0; j < Mosselegali.Num(); j++)
	{
		for (int i = 0; i < MosselegaliAvv.Num(); i++)
		{
			if (Mosselegali[j] == MosselegaliAvv[i])
			{
				Mosselegali.RemoveAt(j);
				
				j--;
			}
		}
		 
	}
	 
	//
	//
	// se il contatore è maggiore di 0, significa che il pezzo ha mosse legali e non è sotto scacco
	//
	return Mosselegali;

	
}


//controllo se la mossa che voglio fare mi espone a un attacco dell'avversario
bool ARandomPlayer::SimulaControMossa(FVector2D TilePosition)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	bool TrovataMossa = false;
	int32 PlayerAvv;

	if (GameMode->CurrentPlayer == 1)
	{
		PlayerAvv = 0;
	}
	else
	{
		PlayerAvv = 1;
	}
	RilevaPezzi(PlayerAvv);

	for (APiece* Piece : PezziAI)
	{

		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());
		
		for (int i = 0; i < Mosselegali.Num(); i++)
		{
			int32 ProprietarioTile = (*GameMode->GField->TileMap.Find(Mosselegali[i]))->GetTileOwner();

			
			if (ProprietarioTile != -1 && ProprietarioTile != PlayerAvv)
			{				
				//se trovo tra le mosse dell'avversario la posizione in cui voglio andare, mi restituise true perchè può mangiarmi
				if ((*GameMode->GField->TileMap.Find(Mosselegali[i]))->GetGridPosition() == TilePosition)
				{
					return true;
				}

			}

		}

	}
	return false;
}

TArray<FVector2D> ARandomPlayer::ControlloScacco(int32 PieceOwner)
{
	//cast di gamemode
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GetWorld()->GetAuthGameMode());
	TArray<FVector2D> Mosselegali = {};


	for (int i = 0; i < Mosselegali.Num(); i++)
	{
		Mosselegali.RemoveAt(i);
	}

	int Avversario;

	if (PieceOwner == 1) //se gioca il computer 
	{
		Avversario = 0; //l'avversario è il computer
	}
	else
	{
		Avversario = 1; //l'avversario è il giocatore umano
	}


	FVector2D RePosition = GameMode->GField->TrovaRe(PieceOwner); //Se bianchi avversario è il nero e viceversa
	//solo per il controllo cambio il giocatore con l'avversario e poi faccio l'inverso

	if (RePosition.X >= 0)
	{
		  Mosselegali =  SimulaMosseControlloScacco(RePosition, Avversario);
		 
		return Mosselegali;
	}
	return Mosselegali;
	}

int ARandomPlayer::ControlloScaccoNum(int32 PieceOwner)
{
	//cast di gamemode
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GetWorld()->GetAuthGameMode());
	TArray<FVector2D> Mosselegali = {};
	for (int i = 0; i < Mosselegali.Num(); i++)
	{
		Mosselegali.RemoveAt(i);
	}
	int Avversario;

	if (PieceOwner == 1) //se gioca il computer 
	{
		Avversario = 0; //l'avversario è il computer
	}
	else
	{
		Avversario = 1; //l'avversario è il giocatore umano
	}


	FVector2D RePosition = GameMode->GField->TrovaRe(PieceOwner); //Se bianchi avversario è il nero e viceversa
	//solo per il controllo cambio il giocatore con l'avversario e poi faccio l'inverso

	 
	return SimulaMosseControlloScaccoNum(RePosition, Avversario);;
}

int ARandomPlayer::SimulaMosseControlloScaccoNum(FVector2D TilePosition, int32 GiocatoreAVV)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());


	FVector2D Mossa = FVector2D(-1, -1);
	int32 Indice = 0;
	TArray<FVector2D> MosselegaliAvv;
	int32 contatoreEND = 0;
	int32 contatoreINI = 0;
	bool scacco = false;

	while (GameMode->GField->Mangiate.size() > 0)
	{
		GameMode->GField->Mangiate.erase(GameMode->GField->Mangiate.begin() + 0); // Rimuovi il primo elemento dall'array
	}

	APiece* Piece = nullptr;
	//Trovo il pezzo che voglio controllare se ha mosse legali
	Piece = GameMode->GField->GetTileAtPosition(TilePosition)->GetPiece();
	//Trovo tutte le mosse legali del pezzo
	TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Piece->GetTile());

	if (Mosselegali.IsEmpty())
	{
		return -9;
	}
	contatoreINI = Mosselegali.Num();
	contatoreEND = Mosselegali.Num();

	// Trovo i pezzi avverssario 
	RilevaPezzi(GiocatoreAVV);


	for (APiece* PieceNew : PezziAI)
	{
		for (FVector2D MossalegAvv : PieceNew->CalculateMoves(PieceNew->GetTile()))
		{
			MosselegaliAvv.Add(MossalegAvv);
		}
	}


	//se il pezzo è sotto scacco, non ha mosse legali e non può muoversi


	for (int j = 0; j < Mosselegali.Num(); j++)
	{
		for (int i = 0; i < MosselegaliAvv.Num(); i++)
		{
			if (Mosselegali[j] == MosselegaliAvv[i])
			{
				Mosselegali.RemoveAt(j);
				contatoreEND = Mosselegali.Num();
				j--;
			}
		}

	}


	//
	//
	// se il contatore è maggiore di 0, significa che il pezzo ha mosse legali e non è sotto scacco
	//
	if (contatoreINI == contatoreEND)
	{
		return 1; //non è in scacco
	}
	else if (contatoreINI > contatoreEND)
	{
		return 2; //è in scacco
	}
	else if (contatoreEND <= 0 )
	{
		return 3; //è in scacco
	}
	else
	{
		return 3; //Scacco matto 
	}


}