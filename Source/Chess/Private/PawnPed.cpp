// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPed.h"

// Sets default values
APawnPed::APawnPed()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Materials/PawnMatW.PawnMatW"));

	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}

	UMaterialInterface* Materiale = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/PawnMatW.PawnMatW"));
	if (Materiale)
	{
		StaticMeshComponent->SetMaterial(0, Materiale);
	}


}

// Called when the game starts or when spawned
void APawnPed::BeginPlay()
{
	Super::BeginPlay();

}

TArray<FVector2D> APawnPed::CalculateMoves(ATile* CurrTile)
{
    AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

    // Inizializziamo l'array delle mosse legali
    TArray<FVector2D> LegalMoves;

    // Otteniamo la posizione attuale del pedone
    FVector2D PawnPosition = CurrTile->GetGridPosition();

    // Definiamo la direzione in cui il pedone si muove in base al colore
    int ForwardDirection = 1;
    int32 Player = CurrTile->GetTileOwner();

    if (Player == 1)
	{
         ForwardDirection = -1;
	}
	

    // Movimento diretto in avanti
    FVector2D ForwardMove = FVector2D(ForwardDirection, 0);
    FVector2D ForwardTwoMoves = ForwardMove * 2;

    // Verifichiamo se il pedone può muoversi in avanti di una o due caselle
    if ((CurrTile->GetGridPosition().X == 1 && CurrTile->GetTileOwner()==0) || (CurrTile->GetGridPosition().X == 6 && CurrTile->GetTileOwner() == 1))
    {
        // Movimento di due caselle in avanti consentito solo dalla posizione iniziale
        FVector2D TwoMovesPosition = PawnPosition + ForwardTwoMoves;
        //devo verificare se la casella davanati è vuota
        FVector2D NextTilePosition = CurrTile->GetGridPosition() + ForwardMove;
        //
        // 
        // DEVO VERIFICARE SE LA CASA DAVANTI E' VUOTA
        // NON SO COME RECUPERARE LA TILE DALLA POSIZIONE
        // 
        // 
        //ATile* NextTile = GameMode->GField->GetRelativeLocationByXYPosition(NextTilePosition);


        if ( CurrTile->GetTileStatus() == ETileStatus::EMPTY)
        {
            LegalMoves.Add(TwoMovesPosition);
        }
    }

    // Movimento di una casella in avanti
    FVector2D OneMovePosition = PawnPosition + ForwardMove;
    if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
    {
        LegalMoves.Add(OneMovePosition);
    }

    // Movimento diagonale per catturare un pezzo avversario
    for (int32 DiagonalOffset : {-1, 1})
    {
        FVector2D DiagonalMove = FVector2D(DiagonalOffset, ForwardDirection);
        FVector2D DiagonalPosition = PawnPosition + DiagonalMove;
        if (CurrTile->GetTileStatus() == ETileStatus::EMPTY && CurrTile->GetTileOwner() != PlayerOwner )
        {
            LegalMoves.Add(DiagonalPosition);
        }
    }

    // Restituiamo l'array delle mosse legali
    return LegalMoves;
}
