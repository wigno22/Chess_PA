// Fill out your copyright notice in the Description page of Project Settings.


#include "King.h"
//#include "Tile.h"


// Sets default values
AKing::AKing()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/KingMatW.KingMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/KingMatW.KingMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void AKing::BeginPlay()
{
	Super::BeginPlay();


	
}


TArray<FVector2D> AKing::CalculateMoves(ATile* CurrTile)
{
    AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

    // Inizializziamo l'array delle mosse legali
    TArray<FVector2D> LegalMoves;

    // Definiamo le direzioni possibili per il re: sopra, sotto, a sinistra, a destra e diagonali
    const TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1),
        FVector2D(1, 1), FVector2D(1, -1), FVector2D(-1, 1), FVector2D(-1, -1)
    };

    // Per ogni direzione possibile
    for (const FVector2D& Direction : Directions)
    {
        // Calcoliamo la posizione della mossa legale
        FVector2D PositionLegalMove = CurrTile->GetGridPosition() + Direction;

        // Verifichiamo se la posizione è valida (all'interno della scacchiera)
        if (PositionLegalMove.X >= 0 && PositionLegalMove.X < 8 &&
            PositionLegalMove.Y >= 0 && PositionLegalMove.Y < 8)
        {
            // Controllo se la tile è vuota o occupata da un pezzo
            int32 ProprietarioTile = (*GameMode->GField->TileMap.Find(PositionLegalMove))->GetTileOwner();
            if (ProprietarioTile == -1 || ProprietarioTile == 1)
            {
                // La tile è vuota o occupata da un pezzo avversario, quindi la mossa è valida
                (*GameMode->GField->TileMap.Find(PositionLegalMove))->bIsValid = true;
                LegalMoves.Add(PositionLegalMove);
            }
        }
    }

    // Restituiamo l'array delle mosse legali
    return LegalMoves;
}

