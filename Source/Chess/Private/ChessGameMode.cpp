// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessGameMode.h"
#include "Chessboard.h"
#include "ChessPlayerController.h"
#include "HumanPlayer.h" 
#include "ChessWidget.h"
#include <RandomPlayer.h>
#include "EngineUtils.h" 






AChessGameMode::AChessGameMode()
{
	PlayerControllerClass = AChessPlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
	FieldSize = 8;
	// get the game instance reference
	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}


void AChessGameMode::BeginPlay()
{

	Super::BeginPlay();


	HUD = CreateWidget<UChessWidget>(GetGameInstance(), ChessWidgetClass);

	if (HUD)
	{
		HUD->AddToViewport(1);
	}




	IsGameOver = false;

	MoveCounter = 0;


	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));

	
		if (ChessboardClass != nullptr)
		{
			GField = GetWorld()->SpawnActor<AChessboard>(ChessboardClass);
			GField->BoardSize = FieldSize;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
		}


	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	
	FVector CameraPos(CameraPosX, CameraPosX, 1100.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// Random Player
	auto* AI = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());

	 
	// AI player = 1
	Players.Add(AI);

	this->ChoosePlayerAndStartGame();

}

void AChessGameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = 0;

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
		
	}
	
	//Players[CurrentPlayer]->OnTurn();
}
 

int32 AChessGameMode::GetNextPlayer(int32 Player) 
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void AChessGameMode::TurnNextPlayer(int32 Player)
{
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	//MoveCounter += 1;
	CurrentPlayer = Player;


	if (CurrentPlayer == 1)
	{
		GameMode->bIsMyTurn = true;


		TArray<APiece*> PezziAI;

		//cast di random player per poter usare i suoi metodi
		ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(Players[Player]);

		
		RandomPlayer->GiocatoreAI(Player);
	}
	
	 
}
  


void AChessGameMode::OnWin()
{
	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AChessboard* Chessboard = Cast<AChessboard>(GField);

	IsGameOver = true;

	if (Winner == 1)
	{
		GameInstance->SetTurnMessage(TEXT("AI Wins!"));
		GameInstance->IncrementScoreAiPlayer();
	}
	else
	{
		GameInstance->SetTurnMessage(TEXT("Human Wins!"));
		GameInstance->IncrementScoreHumanPlayer();
	}


}

void AChessGameMode::AddMossa(FString NamePiece, FVector2D PosFinale, int32 ID, int32 OwnerPed)
{
	HUD->AddMossa(NamePiece, PosFinale, ID, OwnerPed);
}
 