// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Queen.h"
#include "King.h"
#include "Horse.h"
#include "Bishop.h"
#include "GameFramework/Actor.h"
#include "Chessboard.generated.h"

//macro declaration for a dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);


UCLASS()
class CHESS_API AChessboard : public AActor
{
	GENERATED_BODY()
	
public:	

	int32 GloXC = 7;
	int32 GloXG = 7;


	// Sets default values for this actor's properties
	AChessboard();

	// keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	//given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	//fOnReset delegate
	UPROPERTY(BlueprintAssignable)
	FOnReset OnReset;

	//size of the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoardSize;

	//TSubclassof template class that provides UClass* type safety
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATile> TileClass;

	//Tile padding
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CellPadding;

	//Tile size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;


	UPROPERTY(EditAnywhere)
	UMaterialInterface* WhiteMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* BlackMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* GreenMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* RedMaterial;


	//Attributo per la posizione della pedina selezionata
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileAttiva;

	//Metodo per le mosse legali a cui passo la posizione della pedina selezionata
	void ColorLegalMoves(TArray<FVector2D> MosseLegali);

	//metodo per fare la mossa
	void DoMove(FVector2D PosInit, FVector2D PosFin);

	//metodo per resettare mosse legali
	void ResetLegalMoves();

	//Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	//Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// remove all signs from the field
	UFUNCTION(BlueprintCallable)
	void ResetField();

	//generate an empty field
	void GenerateField();

	//metodo per generare la pedina
	virtual void GeneratePiece(int32 x, int32 y);

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetTilePosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

	// return a relative position given (x,y) position
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)const;

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	// checking if is a valid field position
	inline bool IsValidPosition(const FVector2D Position) const;

	// get a line given a begin and end positions
	TArray<int32> GetLine(const FVector2D Begin, const FVector2D End) const;

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;




};
