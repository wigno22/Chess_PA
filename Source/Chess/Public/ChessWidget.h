// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Chessboard.h"

 
#include "Components/ScrollBar.h" // Per utilizzare la classe UScrollBar (per gli eventi di scorrimento)
#include "InputCoreTypes.h" // Per utilizzare tipi di input come FPointerEvent (per eventi del mouse)

#include "Components/ScrollBox.h"
#include "ChessWidget.generated.h"



/**
 * 
 */
UCLASS()
class CHESS_API UChessWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	 UPROPERTY( meta = (BindWidget))
	 UScrollBox* ScrollBox; // Variabile che conterrà il riferimento allo ScrollBox
	 
	 UPROPERTY(EditAnywhere)
	 TSubclassOf<class UMyWidget> MyWidgetEntry;

	// Funzione che aggiunge una mossa al scrollbox
	 UFUNCTION(BlueprintCallable)
	 void AddMossa(FString Mossa, FVector2D PosFin);

	

	 // Crea la funzione di gestione degli eventi di clic del pulsante
	 UFUNCTION(BlueprintCallable)
	 void HandleButtonClick();
		
	 
	 UFUNCTION(BlueprintCallable)
	 void HandleScroll(float ScrollOffset);

	

};

 