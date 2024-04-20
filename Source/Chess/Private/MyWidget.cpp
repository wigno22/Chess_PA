// Fill out your copyright notice in the Description page of Project Settings.
#include "MyWidget.h"
#include "Chessboard.h"
#include "ChessGameMode.h"
#include <string>
#include <ChessWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Components/Button.h"


void UMyWidget::HandleButtonClick()
{
	
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	APiece* PieceQ = nullptr;
	ATile* Tiletemp = nullptr;
	//devo recuperare il testo del pulsante cliccato
	FText TestoButtonText = Button->GetAccessibleText();
	//CONVERTO FTEXT IN STRING
	FString TestoButton = TestoButtonText.ToString();
	//converto FString in std::string
	std::string TestoButtonString = TCHAR_TO_UTF8(*TestoButton);

	size_t end_pos = TestoButtonString.find(":", 2);

	std::string IDString = TestoButtonString.substr(2, end_pos - 2);

	int32 NumMossa = std::stoi(IDString)-1;


	if ((GameMode->GField->Mosse[NumMossa].Player == 0 && GameMode->GField->Mosse[NumMossa].PosFin.Y > 7) || (GameMode->GField->Mosse[NumMossa].Player == 1 && GameMode->GField->Mosse[NumMossa].PosFin.Y < 7))
	{
		GameInstance->SetTurnMessage(TEXT("Selezionare una mossa Bianca!"));
		return;
	}
	for (int32 i = GameMode->GField->Mosse.Num()-1; i >= NumMossa; i--)
	{

		
		if (GameMode->GField->Mosse[i].PosInit.Y == 13)
		{
			GameMode->GField->DistruggiPezzo(GameMode->GField->Mosse[i].PosFin);
		}
		else if (GameMode->GField->Mosse[i].PosInit.Y == 14)
		{ 
			GameMode->GField->DistruggiPezzo(GameMode->GField->Mosse[i].PosFin);
		}
		else
		{
			//devo fare il domove al contrario di tutte le mosse intermedie
			GameMode->GField->ResetMossa(GameMode->GField->Mosse[i].PosFin, GameMode->GField->Mosse[i].PosInit, GameMode->GField->Mosse[i].Player);
		}
			
			GameMode->MoveCounter--;

			// rimuovo l'oggetto appena creato all'array Mosse
			GameMode->GField->Mosse.RemoveAt(i);
			//devo rimuovere anche il bottone corrispondente
			
			GameMode->HUD->RemoveMossa(i);
			

			
	}	



	 
	
}

void UMyWidget::SetData(FString nome, FVector2D PosFin, int32 num, int32 OwnerPed)
{

	// Converte il testo da FString a std::string
	std::string Mossa1;
	Mossa1 = "N:";
	Mossa1 = Mossa1.append(std::to_string(num));
	Mossa1 = Mossa1.append(": ");
	Mossa1 = Mossa1.append(TCHAR_TO_UTF8(*nome));
	Mossa1 = Mossa1.append(" X : ").append(std::to_string(static_cast<int>(PosFin.Y))).append(" Y : ").append(std::to_string(static_cast<int>(PosFin.X)));
	nome = FString(Mossa1.c_str());



	// Imposta il testo del widget di testo
	Text->SetText(FText::FromString(nome));
	Text->Font.Size = 13;
	Text->SetOpacity(1);
	Text->SetColorAndOpacity(FLinearColor::White);
	
	if (OwnerPed == 0 && PosFin.Y <= 7 || OwnerPed == 1 && PosFin.Y  > 7)
	{
	 
		Text->SetColorAndOpacity(FLinearColor::Black);
		FLinearColor WhiteColor = FLinearColor::White;
		// Imposta il colore di sfondo del bottone
		Button->SetBackgroundColor(WhiteColor);

	}
	
	


	Button->OnClicked.AddDynamic(this, &UMyWidget::HandleButtonClick);


}