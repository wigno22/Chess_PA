// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "Tile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Rook.h>
#include <Horse.h> 

// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//set camera as root component
	SetRootComponent(Camera);

	// get the game instance reference
	GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	//default values
	PlayerNumber = -1;

}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHumanPlayer::OnWin()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	//GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	//GameInstance->IncrementScoreHumanPlayer();
}


void AHumanPlayer::OnLose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}



/* Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AHumanPlayer::EseguiMossaUman()
{
	 
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && GameMode->bIsMyTurn)
	{
		//cliccato su tile
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
			{
				AHumanPlayer::OnClickPers(CurrTile);
			}
		else if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			AHumanPlayer::OnClickPers(CurrPiece->GetTile());
		}

	}
	
}
void AHumanPlayer::OnClick()
{
	
	//Super::BeginPlay();
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	AHumanPlayer::EseguiMossaUman();
}

void AHumanPlayer::OnClickPers(ATile* CurrTile)
{
	
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);

	

	//Primo Click
	if (CurrTile->GetTileOwner() == 0)
	{
		//devo resettare la memoria delle mosse valide
		GameMode->GField->ResetLegalMoves();
		//ho la mia tile, prenderò la pedina corrispondente e guarderò le legalmoves
		APiece* Piece = CurrTile->GetPiece();
		TArray<FVector2D> Mosselegali = Piece->CalculateMoves(CurrTile);
		GameMode->GField->TileAttiva = CurrTile->GetGridPosition(); //mi restituisce coppia di coordinate x,y
		GameMode->GField->ColorLegalMoves(Mosselegali, CurrTile);
		GameMode->TurnNextPlayer(0);
	}
	//Secondo Click
	else
	{
		//ho una tile che non appartiene a me ed è valida, devo fare la mossa
		if (CurrTile->bIsValid == true)
		{

			GameMode->GField->DoMove(GameMode->GField->TileAttiva, CurrTile->GetGridPosition(), GameMode->CurrentPlayer);
			//voglio resettare le mosse valide
			GameMode->GField->ResetLegalMoves();
			GameMode->bIsMyTurn = false;
			GameMode->TurnNextPlayer(1);
		//	GameMode->GField->ResetLegalMoves();
		}

	}

}
