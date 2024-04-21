
#pragma once


#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "Chessboard.h"
#include "ChessGameInstance.h"
#include "GameFramework/GameMode.h"
#include "ChessGameMode.generated.h"

class AActor;
class AChessboard;
class UChessWidget;
struct FPosition;

/**
 *
 */
UCLASS()
class CHESS_API AChessGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	//void OnTurn() override;
	void OnWin() ;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UChessWidget> ChessWidgetClass;


	UChessWidget* HUD;
	

	//funzione per aggiungere una mossa al widget
	UFUNCTION(BlueprintCallable)
	void AddMossa(FString NamePiece, FVector2D PosFinale, int32 ID, int32 OwnerPed, bool Blocca);

	 

	// tracks if the game is over
	bool IsGameOver;

	// tracks if the game is over
	int32 Winner;



	// array of player interfaces
	TArray<IPlayerInterface*> Players;
	int32 CurrentPlayer;
	// tracks the number of moves in order to signal a drawn game
	int32 MoveCounter;
	
	//keeps track of turn
	bool bIsMyTurn = true;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AChessboard> ChessboardClass;


	// field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AChessboard* GField;

	AChessGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();

	// get the next player index
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn
	void TurnNextPlayer(int32 Player, FVector2D Mossa);
};
