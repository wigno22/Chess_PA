// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "ChessGameInstance.h"
#include "Piece.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "HumanPlayer.generated.h"

UCLASS()
class CHESS_API AHumanPlayer : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHumanPlayer();

	//camera component
	UCameraComponent* Camera;

	//game instance reference
	UChessGameInstance* GameInstance;

	//puntatore a pezzo selezionato
	APiece* CurrPieceSelected ;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//void OnTurn() override;
	void OnWin() override;
	void OnLose() override;

	void EseguiMossaUman();

	//called on left mouse click
	UFUNCTION()
	void OnClick();

	bool ControlloMossaScacco(FVector2D CurrTile);

	void OnClickPers(ATile* CurrTile);

};
