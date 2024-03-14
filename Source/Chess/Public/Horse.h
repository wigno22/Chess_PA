// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Piece.h"
#include "GameFramework/Pawn.h"
#include "Horse.generated.h"

/**
 *
 */
UCLASS()
class CHESS_API AHorse : public APiece
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHorse();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
