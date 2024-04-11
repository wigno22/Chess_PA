// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameInstance.h"


 

void UChessGameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UChessGameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

int32 UChessGameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UChessGameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

FString UChessGameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UChessGameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}
