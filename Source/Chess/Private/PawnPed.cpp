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
    INT32 MossaVert = 1;

    ////
    AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

    // Inizializziamo l'array delle mosse legali
    TArray<FVector2D> LegalMoves;
    int32 Player = CurrTile->GetTileOwner();
    int32 Direzione = 1;
   
    // Definiamo le direzioni possibili per la torre: sopra, sotto, a sinistra e a destra
    if (Player != 0)
    {
       
        Direzione = -1;
    }
    const FVector2D Direction = { FVector2D(Direzione, 0) };
   


    // Per ogni direzione possibile
    
        if ((CurrTile->GetGridPosition().X == 1 && Player == 0 ) || (CurrTile->GetGridPosition().X == 6 && Player == 1))
        {
	        MossaVert = 2;
           
                
        }
        else
        {
            MossaVert = 1;
        }

        


        // Per ogni passo possibile in quella direzione
        for (int i = 1; i <= MossaVert; i++)
        {
            if ((CurrTile->GetGridPosition().Y - 1)>=0 && CurrTile->GetGridPosition().X + 1<8)
            {
                ATile* TileSX = *GameMode->GField->TileMap.Find(FVector2D(CurrTile->GetGridPosition().X + Direzione, CurrTile->GetGridPosition().Y - 1));

                    if (TileSX->GetTileOwner() != CurrTile->GetTileOwner() && TileSX->GetTileOwner() != -1)
                    {
                        (*GameMode->GField->TileMap.Find(TileSX->GetGridPosition()))->bIsValid = true;
                        LegalMoves.Add(TileSX->GetGridPosition());
                    }
            }
		
            
            if ((CurrTile->GetGridPosition().Y + 1) < 8  && CurrTile->GetGridPosition().X + 1 < 8)
            {
                ATile* TileDX = *GameMode->GField->TileMap.Find(FVector2D(CurrTile->GetGridPosition().X + Direzione, CurrTile->GetGridPosition().Y + 1));

                if (TileDX->GetTileOwner() != CurrTile->GetTileOwner() && TileDX->GetTileOwner() != -1)
                {
                    (*GameMode->GField->TileMap.Find(TileDX->GetGridPosition()))->bIsValid = true;
                    LegalMoves.Add(TileDX->GetGridPosition());
                }
            }
            


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
                else 
                {

                    break;
                }
 
            }
            else
            {
                // Se la posizione è fuori dalla scacchiera, non possiamo muoverci oltre in questa direzione
                break;
            }
        }
    

        // Restituiamo l'array delle mosse legali
        return LegalMoves;

   
}
