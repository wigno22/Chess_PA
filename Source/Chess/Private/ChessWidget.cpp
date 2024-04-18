#include "ChessWidget.h"
#include "MyWidget.h"
#include "Chessboard.h"
#include <string>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "Components/ScrollBar.h" // Per utilizzare la classe UScrollBar (per gli eventi di scorrimento)
#include "InputCoreTypes.h" // Per utilizzare tipi di input come FPointerEvent (per eventi del mouse)

 
void UChessWidget::RemoveMossa(int32 ID)
{
	//devo rimuovere la mossa con ID corrispondente
	//devo rimuovere il bottone corrispondente
	//devo rimuovere il testo

	ScrollBox->RemoveChildAt(ID);
}




void UChessWidget::AddMossa(FString NamePiece, FVector2D PosFin, int32 ID, int32 OwnerPed)
{

	//FString nomePezzo = Pezzo->GetName();
	


	UUserWidget* Widget = CreateWidget(this, MyWidgetEntry);
	
	ScrollBox->AddChild(Widget);

	UMyWidget* Entry = Cast<UMyWidget>(Widget);
	
	
	Entry->SetData(NamePiece, PosFin, ID, OwnerPed);

	
	
	//setare attibuto dell entry che identificherà la mossa(int)
	//al click riesco a risalire al numero della mossa
	//
	
}
 


 