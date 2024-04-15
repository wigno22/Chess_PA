// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "Tile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Rook.h>
#include <Horse.h> 
#include <ChessWidget.h>
#include <RandomPlayer.h>


// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//set camera as root component
	SetRootComponent(Camera);

	// get the game instance reference
	GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	//default values
	PlayerNumber = -1;

}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHumanPlayer::OnWin()
{

	GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	GameInstance->IncrementScoreHumanPlayer();
}


void AHumanPlayer::OnLose()
{
	
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
	GameInstance->IncrementScoreAiPlayer();
}



/* Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AHumanPlayer::EseguiMossaUman()
{
	 
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && GameMode->bIsMyTurn)
	{
		//cliccato su tile
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
			{
				AHumanPlayer::OnClickPers(CurrTile);
			}
		else if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			AHumanPlayer::OnClickPers(CurrPiece->GetTile());
		}

	}
	
}
void AHumanPlayer::OnClick()
{
	
	//Super::BeginPlay();
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	AHumanPlayer::EseguiMossaUman();
}



bool AHumanPlayer::ControlloMossaScacco(FVector2D TileArrivo)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GameMode->Players[1]);
	
	FVector2D TilePartenza = GameMode->GField->TileAttiva;

	FVector2D RePosition = FVector2D(-1, -1);
	std::vector<AChessboard::Mangiata> Mangiate;
	int32 Giocatore = GameMode->CurrentPlayer;
	bool V_scacco = false;
	int32 Avversario = 0;


	
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetTileStatus(-1, ETileStatus::EMPTY);
	

	APiece* PedPartenza = GameMode->GField->GetTileAtPosition(TilePartenza)->GetPiece();
	APiece* PedArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetPiece();
	int32 OwnerArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetTileOwner();
	ETileStatus StatusArrivo = GameMode->GField->GetTileAtPosition(TileArrivo)->GetTileStatus();
	

	GameMode->GField->GetTileAtPosition(TileArrivo)->SetTileStatus(Giocatore, ETileStatus::OCCUPIED);
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetPiece(PedPartenza);



	if (Giocatore == 1) //se gioca il computer 
	{
		Avversario = 0; //l'avversario � il giocatore umano
	}
	else
	{
		Avversario = 1; //l'avversario � il computer
	}


	RePosition = GameMode->GField->TrovaRe(Giocatore); //Se bianchi avversario � il nero e viceversa
	

	if (RePosition.X >= 0)
	{

		Mangiate = RandomPlayer->GiocatoreAIScacco(Avversario);

		for (int i = 0; i < Mangiate.size(); i++)
		{
			if (Mangiate[i].PosPezzoMangiato == RePosition)
			{
				GameInstance->SetTurnMessage(TEXT("La tua ultima mossa ti mette in scacco!"));
				//Faccio comunque la mossa e controllo se � scacco matto
				V_scacco = true;

			}
		}
	}
	
	GameMode->GField->GetTileAtPosition(TilePartenza)->SetTileStatus(Giocatore, ETileStatus::OCCUPIED);
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetTileStatus(OwnerArrivo, StatusArrivo);
	GameMode->GField->GetTileAtPosition(TileArrivo)->SetPiece(PedArrivo);
	return V_scacco;
}






void AHumanPlayer::OnClickPers(ATile* CurrTile)
{
	
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	//cast random player per poter usare i suoi metodi
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GameMode->Players[1]);

	//dichiaro cast di chesswidget per prendere attributi della mia classe e usare funzioni
	UChessWidget* ChessWidget = Cast<UChessWidget>(GameInstance->ChessWidget);

	FVector2D RePosition = FVector2D(-1, -1);
	std::vector<AChessboard::Mangiata> Mangiate;

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	

	//Primo Click
	if (CurrTile->GetTileOwner() == 0)
	{
		if (GameMode->IsGameOver)
		{

			GameInstance->SetTurnMessage(TEXT("Partita Terminata Per Giocare Preme il bottone 'START GAME'"));
									
			return;
		}


		//devo resettare la memoria delle mosse valide
		GameMode->GField->ResetLegalMoves();
		//ho la mia tile, prender� la pedina corrispondente e guarder� le legalmoves
		APiece* Piece = CurrTile->GetPiece();
		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(CurrTile);
		GameMode->GField->TileAttiva = CurrTile->GetGridPosition(); //mi restituisce coppia di coordinate x,y

		FVector2D TileAttiva = CurrTile->GetGridPosition();

		
		//scorro le mosse legali per controllare di non farmi mangiare il re 
		for (int i = 0; i < Mosselegali.Num(); i++)
		{
			if (ControlloMossaScacco(Mosselegali[i]))
			{
				Mosselegali.RemoveAt(i);
				i--;
			}
		}
		


		GameMode->GField->TileAttiva = TileAttiva;



		GameMode->GField->ColorLegalMoves(Mosselegali, CurrTile);
		
		
		GameMode->TurnNextPlayer(0);
	}
	//Secondo Click
	else
	{

		FVector2D TilePartenza = GameMode->GField->TileAttiva;
		FVector2D TileArrivo = CurrTile->GetGridPosition();
		int32 Giocatore = GameMode->CurrentPlayer;
		bool V_scacco = false;
		int32 Avversario = 0;

		if (Giocatore == 1) //se gioca il computer 
		{
			Avversario = 0; //l'avversario � il giocatore umano
		}
		else
		{
			Avversario = 1; //l'avversario � il computer
		}



		//ho una tile che non appartiene a me ed � valida, devo fare la mossa
		if (CurrTile->bIsValid == true)
		{

				RePosition = GameMode->GField->TrovaRe(Giocatore); //Se bianchi avversario � il nero e viceversa
			
			
			if (RePosition.X >= 0)
			{

					Mangiate = RandomPlayer->GiocatoreAIScacco(Avversario);


				
				for (int i = 0; i < Mangiate.size(); i++)
				{
					if (Mangiate[i].PosPezzoMangiato == RePosition)
					{
						GameInstance->SetTurnMessage(TEXT("Scacco!"));
						//Faccio comunque la mossa e controllo se � scacco matto
						GameMode->GField->DoMove(TilePartenza, TileArrivo, Giocatore);
						V_scacco = true;

						RePosition = GameMode->GField->TrovaRe(Giocatore);
						

						if (RePosition.X >= 0)
						{
							Mangiate = RandomPlayer->GiocatoreAIScacco(Avversario);
							for (int j = 0; j < Mangiate.size(); j++)
							{
								if (Mangiate[j].PosPezzoMangiato == RePosition)
								{
									GameInstance->SetTurnMessage(TEXT("Scacco!"));
									GameMode->GField->DoMove(TileArrivo, TilePartenza, Giocatore);
									V_scacco = true;
									return;
								}
								 
							}
						}


					}
				}	
				RandomPlayer->RilevaPezzi(0);
			}


			if (!V_scacco)
			{
				 
				GameMode->GField->DoMove(TilePartenza, TileArrivo, Giocatore);
			}



			RePosition = GameMode->GField->TrovaRe(Giocatore); //Se bianchi avversario � il nero e viceversa


			if (RePosition.X >= 0)
			{

				Mangiate = RandomPlayer->GiocatoreAIScacco(Avversario);

				for (int i = 0; i < Mangiate.size(); i++)
				{
					if (Mangiate[i].PosPezzoMangiato == RePosition)
					{
						GameInstance->SetTurnMessage(TEXT("La tua ultima mossa ti mette in scacco!"));
						//Faccio comunque la mossa e controllo se � scacco matto
						GameMode->GField->DoMove(TileArrivo, TilePartenza, Giocatore);
						V_scacco = true;
						return;
					}
				}
			}

			GameInstance->SetTurnMessage(TEXT("  ---  "));
			//voglio resettare le mosse valide
			GameMode->GField->ResetLegalMoves();

			GameMode->bIsMyTurn = false;
			GameMode->TurnNextPlayer(1);
		
			
		}

	}

}
