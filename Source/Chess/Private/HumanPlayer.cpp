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
	//GameInstance = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//default values
	PlayerNumber = -1;

}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
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

/*
void AHumanPlayer::OnTurn()
{
		bIsMyTurn = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("My Turn"));
	//	GameInstance->SetTurnMessage(TEXT("Your Turn"));
}


void AHumanPlayer::OnWin()
{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win"));
	//	GameInstance->SetTurnMessage(TEXT("You Win"));
	//	GameInstance->IncrementScoreHumanPlayer();
}

void AHumanPlayer::OnLose()
{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose"));
	//	GameInstance->SetTurnMessage(TEXT("You Lose"));
}

*/
void AHumanPlayer::OnClick()
{
	//prendo attributi gamemode qui per usarli nei due if
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (Hit.bBlockingHit && bIsMyTurn)
	{
		//cliccato su tile
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			

				//Primo Click
				if (CurrTile->GetTileOwner() == 0 )
				{
					//devo resettare la memoria delle mosse valide
					GameMode->GField->ResetLegalMoves();
					//ho la mia tile, prender� la pedina corrisspondente e guarder� le legalmoves
					APiece* Piece = CurrTile->GetPiece();
					TArray<FVector2D> Mosselegali = Piece->CalculateMoves(Cast<ATile>(Hit.GetActor()));
					GameMode->GField->TileAttiva = CurrTile->GetGridPosition(); //mi restituisce coppia di coordinate x,y
					GameMode->GField->ColorLegalMoves(Mosselegali);

				}
				//Secondo Click
				else
				{
					//ho una tile che non appartiene a me ed � valida, devo fare la mossa
					if (CurrTile->bIsValid== true)
					{					
					GameMode->GField->DoMove(GameMode->GField->TileAttiva, CurrTile->GetGridPosition());


					//voglio resettare le mosse valide
					GameMode->GField->ResetLegalMoves();
					}
				    
				}
			
		}

		 
		else if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{

			FVector2D PosPedina =  CurrPiece->GetGridPosition();
			ATile* FoundTile = nullptr;


			if (*GameMode->GField->TileMap.Find(PosPedina))
			{
				FoundTile = *GameMode->GField->TileMap.Find(PosPedina);
			

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Trovata"));
			}





		/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Cliccato Pedone"));

			CurrPieceSelected = CurrPiece;
						
			GameMode->GField->TileAttiva = CurrPiece->GetGridPosition(); //mi restituisce coppia di coordinate x,y
			


			if (CurrPiece-> GetOwner() == 0 )
			{
				GameMode->GField->LegalMoves();

				//chiamo DoMove per eseguire la mossa
				GameMode->GField->DoMove(GameMode->GField->TileAttiva, CurrPiece->GetGridPosition());

				//cambiare turno di gioco
			}
*/
			
		}

	}


}

