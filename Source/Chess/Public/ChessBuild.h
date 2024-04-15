
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Tile.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessBuild.generated.h"



UCLASS()
class CHESS_API UChessBuild : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UChessBuild();

/*
public:
	//metodo per generare la pedina
	virtual void GeneratePiece(int32 x, int32 y) ;
	APiece* CreaPedone(int32 Colore, FVector Position);
	APiece* CreaRegina(int32 Colore, FVector Position);
	APiece* CreaAlfiere(int32 Colore, FVector Position, int32 y);
	APiece* CreaTorre(int32 Colore, FVector Position, int32 y);
	APiece* CreaCavallo(int32 Colore, FVector Position, int32 y);

*/	


};


