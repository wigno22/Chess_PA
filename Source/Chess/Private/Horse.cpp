// Fill out your copyright notice in the Description page of Project Settings.


#include "Horse.h"

// Sets default values
AHorse::AHorse()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/HorseMatW.HorseMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/HorseMatW.HorseMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void AHorse::BeginPlay()
{
	Super::BeginPlay();

}


TArray<FVector2D> AHorse::CalculateMoves(ATile* CurrTile)
{
    AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

    // Inizializziamo l'array delle mosse legali
    TArray<FVector2D> LegalMoves;

    // Definiamo le direzioni possibili per il cavallo
    const TArray<FVector2D> KnightMoves = {
        FVector2D(1, 2), FVector2D(2, 1),
        FVector2D(2, -1), FVector2D(1, -2),
        FVector2D(-1, -2), FVector2D(-2, -1),
        FVector2D(-2, 1), FVector2D(-1, 2)
    };

    // Per ogni possibile mossa del cavallo
    for (const FVector2D& Move : KnightMoves)
    {
        // Calcoliamo la posizione della mossa legale
        FVector2D PositionLegalMove = CurrTile->GetGridPosition() + Move;

        // Verifichiamo se la posizione è valida (all'interno della scacchiera)
        if (PositionLegalMove.X >= 0 && PositionLegalMove.X < 8 &&
            PositionLegalMove.Y >= 0 && PositionLegalMove.Y < 8)
        {
            // Controllo se la tile è vuota o occupata da un pezzo
            int32 TileOwner = (*GameMode->GField->TileMap.Find(PositionLegalMove))->GetTileOwner();
            if (TileOwner == -1)
            {
                // La tile è vuota, quindi la mossa è valida
                (*GameMode->GField->TileMap.Find(PositionLegalMove))->bIsValid = true;
                LegalMoves.Add(PositionLegalMove);
            }
            else if (TileOwner == 0)
            {
                // La tile è occupata da un pezzo amico, quindi non possiamo muoverci su questa posizione
                continue;
            }
            else if (TileOwner == 1)
            {
                // La tile è occupata da un pezzo avversario, la mossa è valida
                (*GameMode->GField->TileMap.Find(PositionLegalMove))->bIsValid = true;
                LegalMoves.Add(PositionLegalMove);
            }
        }
    }

    // Restituiamo l'array delle mosse legali
    return LegalMoves;
}