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
	ScrollBox->RemoveChildAt(ID);
}




void UChessWidget::AddMossa(FString NamePiece, FVector2D PosFin, int32 ID, int32 OwnerPed, bool Blocca)
{
	UUserWidget* Widget = CreateWidget(this, MyWidgetEntry);
	
	ScrollBox->AddChild(Widget);

	UMyWidget* Entry = Cast<UMyWidget>(Widget);
	
	Entry->SetData(NamePiece, PosFin, ID, OwnerPed,  Blocca);

}
 


 