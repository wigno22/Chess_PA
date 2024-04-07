// Fill out your copyright notice in the Description page of Project Settings.


#include "Queen.h"


// Sets default values
AQueen::AQueen()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/QueenMatW.QueenMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/QueenMatW.QueenMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void AQueen::BeginPlay()
{
	Super::BeginPlay();

}

TArray<FVector2D> AQueen::CalculateMoves(ATile* CurrTile)
{
    AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

    int32 Player = CurrTile->GetTileOwner();

    // Inizializziamo l'array delle mosse legali
    TArray<FVector2D> LegalMoves;

    // Definiamo le direzioni possibili per la regina: sopra, sotto, a sinistra, a destra, e diagonali
    const TArray<FVector2D> Directions = {
        FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1),
        FVector2D(1, 1), FVector2D(1, -1), FVector2D(-1, 1), FVector2D(-1, -1)
    };

    // Per ogni direzione possibile
    for (const FVector2D& Direction : Directions)
    {
        // Per ogni passo possibile in quella direzione
        for (int i = 1; i < 8; i++)
        {
            // Calcoliamo la posizione della mossa legale
            FVector2D PositionLegalMove = CurrTile->GetGridPosition() + Direction * i;

            // Verifichiamo se la posizione è valida (all'interno della scacchiera)
            if (PositionLegalMove.X >= 0 && PositionLegalMove.X < 8 &&
                PositionLegalMove.Y >= 0 && PositionLegalMove.Y < 8)
            {
                // Controllo se la tile è vuota o occupata da un pezzo
                int32 ProprietarioTile = (*GameMode->GField->TileMap.Find(PositionLegalMove))->GetTileOwner();
                if (ProprietarioTile == -1)
                {
                    // La tile è vuota, quindi la mossa è valida
                    (*GameMode->GField->TileMap.Find(PositionLegalMove))->bIsValid = true;
                    LegalMoves.Add(PositionLegalMove);
                }
                else if (ProprietarioTile == Player)
                {
                    // La tile è occupata da un pezzo amico, quindi non possiamo muoverci su questa posizione
                    break;
                }
                else if (ProprietarioTile != Player)
                {
                    // La tile è occupata da un pezzo avversario, la mossa è valida ma non possiamo muoverci oltre
                    (*GameMode->GField->TileMap.Find(PositionLegalMove))->bIsValid = true;
                    LegalMoves.Add(PositionLegalMove);
                    break;
                }
            }
            else
            {
                // Se la posizione è fuori dalla scacchiera, non possiamo muoverci oltre in questa direzione
                break;
            }
        }
    }

    // Restituiamo l'array delle mosse legali
    return LegalMoves;
}