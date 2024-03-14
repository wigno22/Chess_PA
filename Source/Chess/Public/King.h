// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "King.generated.h"

UCLASS()
class CHESS_API AKing : public APiece
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKing();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	//virtual void Move(FVector newPosition) override;

public:	
	/* Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	*/
};
