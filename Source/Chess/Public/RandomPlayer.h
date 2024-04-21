// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "ChessGameMode.h"
#include "Chessboard.h"
#include "vector"
#include "GameFramework/Pawn.h"
#include "RandomPlayer.generated.h"


class AChessboard;

UCLASS()
class CHESS_API ARandomPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARandomPlayer();

	TArray<APiece*> PezziAI;

	//array di pezzi che possono muoversi per poi sceglierne uno e muoverlo
	TArray<FVector2D> MossePossible;

	std::vector<AChessboard::Mangiata> Mangiate;

	//game instance reference
	UChessGameInstance* GameInstance;

	std::vector<AChessboard::Spostato> MossaRandom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void OnTurn() override;
	//virtual void OnWin() override;
	//virtual void OnLose() override;

	void GiocatoreAI(int32 Player,FVector2D Mossa);
	TArray<APiece*> RilevaPezzi(int32 Player);

	void SimulaMosse(FVector2D Mossa);
	bool SimulaControMossa(FVector2D TilePosition);
	TArray<FVector2D> SimulaMosseControlloScacco(FVector2D TilePosition,  int32 GiocatoreAVV);
	TArray<FVector2D> ControlloScacco(int32 PieceOwner);

	int SimulaMosseControlloScaccoNum(FVector2D TilePosition, int32 GiocatoreAVV);
	int ControlloScaccoNum(int32 PieceOwner);
};
