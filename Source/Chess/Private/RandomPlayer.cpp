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

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ARandomPlayer::GiocatoreAI(int32 Player, FVector2D Mossa)
{

	RilevaPezzi(Player);
	SimulaMosse(Player);
	
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

TMap <APiece*, TArray<FVector2D>> ARandomPlayer::MossePossibiliGioc(int32 Player)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	TArray<FVector2D> MosseTotaliBlack;
	
	//svuoto la mappa
	for (auto& Elem : MappaPezzi)
	{
		Elem.Value.Empty();
	}
	//svuoto l'array
	MosseTotaliBlack.Empty();


	PezziAI = RilevaPezzi(Player);
	//scorro tutti i pezzi del computer e trovo le mosse legali

	auto it = PezziAI.CreateIterator();
	//scorro tutti i pezzi del computer
	while (it) {

		APiece* Piece = *it;

		for (int i = 0; i < MosselegaliPedina.Num(); i++)
		{
			MosselegaliPedina.RemoveAt(i);
		}

		MosselegaliPedina = Piece->CalculateMoves(Piece->GetTile());
		//controllo se la mossa che voglio fare mi espone a un attacco dell'avversario
		for (int32 i = 0; i < MosselegaliPedina.Num() ; i++)
		{
		
			
			//QUESTA FUNZIONE MI CAMBIA I PEZZI
			if (ControlloMossaScacco(MosselegaliPedina[i], Player, Piece))
			{
				(*GameMode->GField->TileMap.Find(MosselegaliPedina[i]))->bIsValid = false;
				MosselegaliPedina.RemoveAt(i);
				i--;
			}

		}
		MosseTotaliBlack.Append(MosselegaliPedina);
		MappaPezzi.Add(Piece, MosselegaliPedina);
		++it;
	}
	// se in mappapezzi non ho mosse legali, il giocatore è in scacco matto
	if (MosseTotaliBlack.Num() == 0)
	{
		GameInstance->SetTurnMessage(TEXT("Scacco Matto!"));
		GameMode->CurrentPlayer = 0;
		GameMode->OnWin();

	}
	

	return MappaPezzi;
}


void ARandomPlayer::SimulaMosse(int32 Player)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	
	TArray<FVector2D> Mosselegali;
	bool TrovataMossa = false;	

	while (GameMode->GField->Mangiate.size() > 0)
	{
		GameMode->GField->Mangiate.erase(GameMode->GField->Mangiate.begin() + 0); // Rimuovi il primo elemento dall'array
	}


	//per ogni pezzo controllo le mosse con colorlegalmoves per capire quali possono mangiare
	//scorro dunque la mappapezzi e passo le mosse di ogni pezzo a colorlegalmoves
	MossePossibiliGioc(Player);
	
	for (auto& Pair:MappaPezzi)
	{
		
		//svuo l'array delle mosse legali
		Mosselegali.Empty();

		GameMode->GField->ResetLegalMoves();

		APiece* Pezzotemp = Pair.Key;
		 Mosselegali = Pair.Value;
		 
		

		Mosselegali = MappaPezzi[Pezzotemp];
		GameMode->GField->TileAttiva = Pezzotemp->GetTile()->GetGridPosition();
		ATile* CurrTile = Pezzotemp->GetTile();



		if (Pezzotemp != nullptr && CurrTile->GetPiece() != nullptr && CurrTile->GetTileOwner() == GameMode->CurrentPlayer)
		{
			if (GameMode->GField->ColorLegalMoves(Mosselegali, CurrTile))
			{
				TrovataMossa = true;
			}
		}
		
	}

	FVector2D MossaINI = FVector2D(-1, -1);
	FVector2D MossaEND = FVector2D(-1, -1);
	int UserOwner ;


	if (TrovataMossa && V_scacco == false)
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
					//GameMode->CurrentPlayer = 0;
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

				GameMode->CurrentPlayer = 0;
				break;
			}

		}

	}



	if (!TrovataMossa || TrovataMossa && V_scacco == true)
	{
	
		AChessboard::Spostato Mossa{};
		PezziAI = RilevaPezzi(GameMode->CurrentPlayer);

		MossaRandom.clear();

		for (auto& Pair : MappaPezzi)
		{
			APiece* Piece = Pair.Key;

			MosselegaliPedina = MappaPezzi[Piece];
			
			GameMode->GField->TileAttiva = Piece->GetTile()->GetGridPosition(); //mi restituisce coppia di coordinate x,y

			//mossa di partenza del pezzo
			Mossa.PosInit = Piece->GetTile()->GetGridPosition();

			if (!MosselegaliPedina.IsEmpty())
			{
				//Faccio un for sulle mosselegali
				for (int i = 0; i < MosselegaliPedina.Num(); i++)
				{
					Mossa.Pezzo = Piece;
					Mossa.PosFin = MosselegaliPedina[i];
					Mossa.Player = GameMode->CurrentPlayer;
					//devo controllare che mosselegali non sia vuota 
					
					if (Mossa.Pezzo != nullptr )
					{
						MossaRandom.push_back(Mossa);
					}
					
				}
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

		}
		
	}

	
	V_scacco = false;
	GameMode->GField->ResetLegalMoves();
	GameMode->CurrentPlayer = 0;
	
}



bool ARandomPlayer::ControlloMossaScacco(FVector2D TileArrivo, int32 Giocatore, APiece* Pezzo)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	
	FVector2D TilePartenza = Pezzo->GetGridPosition();

	FVector2D RePosition = FVector2D(-1, -1);
	
	 V_scacco = false;
	int32 Avversario = 0;

	APiece* PedPartenza = GameMode->GField->GetTileAtPosition(TilePartenza)->GetPiece();
	APiece* PedArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetPiece();

	int32 OwnerArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetTileOwner();

	ETileStatus StatusArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetTileStatus();

	GameMode->GField->GetTileAtPosition(TilePartenza)->PlayerOwner = -1;
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetTileStatus(-1, ETileStatus::EMPTY);
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetPiece(nullptr);


	GameMode->GField->GetTileAtPosition(TileArrivo)->PlayerOwner = Giocatore;
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetTileStatus(Giocatore, ETileStatus::OCCUPIED);
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetPiece(PedPartenza);



	if (Giocatore == 1) //se gioca il computer 
	{
		Avversario = 0; //l'avversario è il giocatore umano
	}
	else
	{
		Avversario = 1; //l'avversario è il computer
	}


	RePosition = GameMode->GField->TrovaRe(Giocatore); //Se bianchi avversario è il nero e viceversa


	if (RePosition.X >= 0)
	{
		if (SimulaControMossa(RePosition))

		{
			GameInstance->SetTurnMessage(TEXT("La tua ultima mossa ti mette in scacco!"));
			V_scacco = true;
		}

	}


	GameMode->GField->GetTileAtPosition(TilePartenza)->PlayerOwner = Giocatore;
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetTileStatus(Giocatore, ETileStatus::OCCUPIED);
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetPiece(PedPartenza);


	GameMode->GField->GetTileAtPosition(TileArrivo)->PlayerOwner = OwnerArrivo;
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetTileStatus(OwnerArrivo, StatusArrivo);
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetPiece(PedArrivo);
	return V_scacco;
}



//controllo se la mossa che voglio fare mi espone a un attacco dell'avversario
bool ARandomPlayer::SimulaControMossa(FVector2D TilePosition)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	bool TrovataMossa = false;
	bool Acc=false;
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
					Acc = true;
				}

			}

		}

	}
	RilevaPezzi(GameMode->CurrentPlayer);
	return Acc;
}
