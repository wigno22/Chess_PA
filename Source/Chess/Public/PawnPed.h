// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "ChessGameMode.h"
#include "GameFramework/Pawn.h"
#include "PawnPed.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API APawnPed : public APiece
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	APawnPed();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual TArray<FVector2D> CalculateMoves(ATile* CurrTile) override;

};
