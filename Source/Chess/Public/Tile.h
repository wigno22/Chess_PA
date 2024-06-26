// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class APiece;

UENUM()
enum class ETileStatus : uint8
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
};

UCLASS()
class CHESS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	bool bIsValid;

	bool bIsOut = false;

	// The tile's status
	void SetTileStatus(const int32 TileOwner, const ETileStatus TileStatus);

	//get the tile's status
	ETileStatus GetTileStatus();

	//get the tile's owner
	int32 GetTileOwner();

	//get the Piece on the Tile
	APiece* GetPiece();

	//set the Piece on the Tile
	void SetPiece(APiece* InPiece);

	//Set the position (X,Y)
	void SetGridPosition(const double Inx, const double Iny);

	FVector2D GetGridPosition();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//dichiaro scene component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	//dichiaro mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	//tile status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	//Player owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerOwner;

	//tile position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D GridPosition;

	//attributo per tenere conto del pezzo presente sulla tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APiece* Piece;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
