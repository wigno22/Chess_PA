// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "Tile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
			if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				
				//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
				FVector Position = CurrTile->GetActorLocation();
				Position.Z = 5;

				//salvo posizione della pedina da passare alla cheesboard
				FVector2D PositionXY = CurrTile->GetGridPosition();


				//controllo se la posizione è valida
				//booleano che mi dice se la posizione è valida
				//passo alla cheesboard per controllare

				if (CurrTile->bIsValid == true)
				{
					if (CurrTile->GetTileOwner() == 1)
					{
					//stampo messaggio di debug
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Avversario Mangiato"));
					}
					else
					{
						CurrPieceSelected->SetActorLocation(Position);
					}
					

					//cambiare turno di gioco

				}
				//devo resettare la memoria delle mosse valide
				GameMode->GField->ResetLegalMoves();


				
			}
		}
		//cliccato su pezzo
		else if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Cliccato Pedone"));

			CurrPieceSelected = CurrPiece;

			

			GameMode->GField->TileAttiva = CurrPiece->GetGridPosition(); //mi restituisce coppia di coordinate x,y
			
			GameMode->GField->LegalMoves();



			/*salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
			 ATile* TilePedone = Cast<ATile>(Hit.GetActor());
			FVector Location = CurrPiece->GetActorLocation();
			Location.Z = 4.940656458412e-324;
			UWorld* World = GetWorld();

			TArray<FOverlapResult> OverlappingActors;
			World -> OverlapMultiByObjectType(OverlappingActors, Location , FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), FCollisionShape::MakeSphere(100.0f));
			*/
		}

	}


}

