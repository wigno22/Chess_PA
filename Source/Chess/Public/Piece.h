// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
//#include "Chessboard.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Piece.generated.h"

UCLASS()

//class ATile;
class CHESS_API APiece : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APiece();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY (EditAnywhere)
	FVector position;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;


	//dichiaro scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	//dichiaro mesh component
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	//size of the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoardSize;

	//TSubclassof template class that provides UClass* type safety
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;


	//Tile size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;


	//tile position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GridPosition;

	//tile owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATile* CurrentTile;

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly)
	int32 Weight;

	int32 GetWeight();

	void SetWeight(int32 NewWeight);

	//Get the tile where the piece is
	ATile* GetTile();

	//Set the tile where the piece is
	void SetCurrentTile(ATile* NewTile);

	//Set the position (X,Y)
	void SetGridPosition(const double Inx, const double Iny);

	//Get the position (X,Y)
	FVector2D GetGridPosition();

	//mi servirà per muovere la pedina
	virtual void Move(FVector newPosition);

	//dichiaro get e set owner della pedina
	int32 GetPlayerOwner();

	void SetPlayerOwner(int32 NewOwner);

	virtual TArray<FVector2D> CalculateMoves(ATile* CurrTile);

	UFUNCTION()
	void SelfDestroy();
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
