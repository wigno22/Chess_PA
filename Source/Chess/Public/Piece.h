// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
//#include "Chessboard.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Piece.generated.h"

UCLASS()
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

	//attributi della classe tile che mi serviranno per generare la pedina
	// //
	//size of the board
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BoardSize;

	//TSubclassof template class that provides UClass* type safety
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;

	/*Tile padding
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CellPadding;

	//Tile size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;
	*/

	//Tile size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;


	//tile position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GridPosition;

	// Definizione di PieceArray e PieceMap
	TArray<APiece*> PieceArray;

	TMap<FVector2D, APiece*> PieceMap;

	//Set the position (X,Y)
	void SetGridPosition(const double Inx, const double Iny);

	//
	// fine attributi presi da tile 



	//mi servirà per muovere la pedina
	virtual void Move(FVector newPosition);




	UFUNCTION()
	void SelfDestroy();
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
