// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "Tile.h"
#include "ChessGameMode.h"
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
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				CurrTile->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);
				FVector SpawnPosition = CurrTile->GetActorLocation();
				AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
				GameMode->SetCellSign(PlayerNumber, SpawnPosition);
				//riabilitare per il multiplayer
				// bIsMyTurn = false;
				//log in cui mi viene detto quale attore è stato cliccato
				UE_LOG(LogTemp, Warning, TEXT("Tile clicked %s"), *Hit.GetActor()->GetName());

				//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
				FVector Position = CurrTile->GetActorLocation();
				Position.Z = 5;
				//FRotator Rotation = FRotator(0, 90, 0);
				APiece* PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);

				const float PawnScale = 120 / 110.0f;
				PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
				//PieceObj->SetGridPosition(x, y);

				// Applica la rotazione dopo che l'oggetto è stato creato con successo
				FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
				PieceObj->AddActorLocalRotation(Rotation);
			}
		}
		/*cliccato su pezzo
		else if (APiece* CurrPiece = Cast<APiece>(Hit.GetActor()))
		{
			if (CurrPiece->GetPieceOwner() == PlayerNumber)
			{
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));
				CurrPiece->SetPieceOwner(PlayerNumber);
				FVector SpawnPosition = CurrPiece->GetActorLocation();
				AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
				GameMode->SetCellSign(PlayerNumber, SpawnPosition);
				bIsMyTurn = false;
			}
		}*/

	}


}

