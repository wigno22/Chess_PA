#include "ChessWidget.h"
#include "MyWidget.h"
#include <string>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "Components/ScrollBar.h" // Per utilizzare la classe UScrollBar (per gli eventi di scorrimento)
#include "InputCoreTypes.h" // Per utilizzare tipi di input come FPointerEvent (per eventi del mouse)
 





void UChessWidget::AddMossa(FString Mossa, FVector2D PosFin)
{

	UUserWidget* Widget = CreateWidget(this, MyWidgetEntry);
	ScrollBox->AddChild(Widget);

	/*
	// Crea un nuovo widget di testo
	UTextBlock* TextBlock = NewObject<UTextBlock>(UTextBlock::StaticClass());
	UButton* Button = NewObject<UButton>(UButton::StaticClass());

	Button->SetVisibility(ESlateVisibility::Visible);
	Button->IsFocusable = true;
	

	// Imposta la dimensione del font del widget di testo
	TextBlock->Font.Size = 20;	
	
	// Converte il testo da FString a std::string
	std::string Mossa1 = TCHAR_TO_UTF8(*Mossa);
	Mossa1 = Mossa1.append(" X : ").append(std::to_string(static_cast<int>(PosFin.Y))).append(" Y : ").append(std::to_string(static_cast<int>(PosFin.X)));
	Mossa = FString(Mossa1.c_str());

	// Imposta il testo del widget di testo
	TextBlock->SetText(FText::FromString(Mossa));

	Button->InsertChildAt(0, TextBlock);
	// Aggiungi il widget di testo al ListView
	 // Associa la funzione di gestione degli eventi alla delegata OnClicked del pulsante
	Button->OnClicked.AddDynamic(this, &UChessWidget::HandleButtonClick);


	ScrollBox->AddChild(Button); // Aggiungi il pulsante alla ScrollBox
	ScrollBox->ScrollBarVisibility = ESlateVisibility::Visible;
	*/

	
}
 


// Crea la funzione di gestione degli eventi di clic del pulsante
void UChessWidget::HandleButtonClick()
{
	// Aggiungi qui il codice da eseguire quando il pulsante viene cliccato
	UE_LOG(LogTemp, Warning, TEXT("Il pulsante è stato cliccato!"));
	
}

void UChessWidget::HandleScroll(float ScrollOffset)
{
	// Gestisci lo scorrimento della ScrollBox
	UE_LOG(LogTemp, Warning, TEXT("Il pulsante è stato cliccato!"));
}

 