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

	class Spostamento
	{
	public:
		FVector2D PosInit;
		FVector2D PosFin;
		APiece* Pezzo;
		int32 Player;
	};

	// Sets default values for this pawn's properties
	ARandomPlayer();

	TArray<APiece*> PezziAI;

	//array di pezzi che possono muoversi per poi sceglierne uno e muoverlo
	TArray<Spostamento> MossePossible;

	std::vector<AChessboard::Mangiata> Mangiate;

	//game instance reference
	UChessGameInstance* GameInstance;

	std::vector<AChessboard::Spostato> MossaRandom;

	TMap <APiece*, TArray<FVector2D>> MappaPezzi;
	TArray<FVector2D> MosselegaliPedina;

	bool V_scacco = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TMap<APiece*, TArray<FVector2D>> MossePossibiliGioc(int32 Player);

	void GiocatoreAI(int32 Player,FVector2D Mossa);
	TArray<APiece*> RilevaPezzi(int32 Player);

	void SimulaMosse(int32 Player);
	bool ControlloMossaScacco(FVector2D TileArrivo, int32 Giocatore, APiece* Pezzo);


	bool SimulaControMossa(FVector2D TilePosition);
	
};
