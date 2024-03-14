// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../../../../../Program Files/Epic Games/UE_5.2/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputLibrary.h"
#include "ChessPlayerController.generated.h"

UCLASS()
class CHESS_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	public:

	AChessPlayerController();

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* ChessContext;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputAction* ClickAction;

	void ClickOnGrid();

	protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


};
