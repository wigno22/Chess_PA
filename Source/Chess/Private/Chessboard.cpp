// Fill out your copyright notice in the Description page of Project Settings.

//#include "BaseSign.h"
#include "Chessboard.h"
#include "Kismet/GameplayStatics.h"
#include <Rook.h>
#include <Horse.h>
#include <Bishop.h>
#include <King.h>
#include <Queen.h>
#include <PawnPed.h>
#include <RandomPlayer.h>


// Sets default values
AChessboard::AChessboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Size of the chessboard
	BoardSize = 8;
	//Tile size
	TileSize = 120;
	//Tile padding
	CellPadding = 0;

}

bool  AChessboard::ColorLegalMoves(TArray<FVector2D> MosseLegali  ,ATile* TileDef)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GetWorld()->GetAuthGameMode());
	
	//ciclo per colorare le mosse legali
	for (int i=0; i<MosseLegali.Num(); i++)
	{
		int32 ProprietarioTile = (*TileMap.Find(MosseLegali[i]))->GetTileOwner();

		(*TileMap.Find(MosseLegali[i]))->StaticMeshComponent->SetMaterial(0, GreenMaterial);
		
		if (ProprietarioTile != -1 && ProprietarioTile != GameMode->CurrentPlayer)
		{
			(*TileMap.Find(MosseLegali[i]))->StaticMeshComponent->SetMaterial(0, RedMaterial);
			//RandomPlayer->MossePossible.Add(MosseLegali[i]);

			//APiece* PezzoDaMangiare = (*TileMap.Find(MosseLegali[0]))->GetPiece();

			if (GameMode->CurrentPlayer == 1)
			{
				//prima di mangiare guardo se ho la possibilit� di mangiare due pedine devo mangiare quella pi� pesante
				
				/*for (int32 j=0; j< RandomPlayer->MossePossible.Num(); j++ )
				{
					//non devo passare le mosse legali ma solo quelle per cui posso mangiare
					
					if ((*TileMap.Find(RandomPlayer->MossePossible[j]))->GetPiece()->GetWeight() > PezzoDaMangiare->GetWeight())
					{
						PezzoDaMangiare = (*TileMap.Find(RandomPlayer->MossePossible[j]))->GetPiece();
					}
				}*/

			//	FVector2D PosPezzoDaMangiare = PezzoDaMangiare->GetGridPosition();
				GameMode->GField->DoMove(TileDef->GetGridPosition(), MosseLegali[i], GameMode->CurrentPlayer);
				ResetLegalMoves();
				return true;
			}
		}

	}
	return false;


}

void AChessboard::ResetLegalMoves()
{
	for (ATile* Obj : TileArray)
	{
		Obj->bIsValid = false;
		FVector2D Position = Obj->GetGridPosition();
		int32 x = static_cast<int32>(Position.X);
		int32 y = static_cast<int32>(Position.Y);

		//resetto colore tile
		if ((x + y) % 2 == 0)
		{
			Obj->StaticMeshComponent->SetMaterial(0, WhiteMaterial);
		}
		else
		{
			Obj->StaticMeshComponent->SetMaterial(0, BlackMaterial);
		}
	}
}

//funzione per eseguire la mossa, chiamata dopo aver controllato le validmoves
void AChessboard:: DoMove(FVector2D PosInit, FVector2D PosFin, int32 CurrentPlayer)
{
	//prendo il pezzo nella posizione iniziale e lo sposto nella posizione finale
	//poi metto la posizione iniziale a vuota e la posizione finale occupata

	APiece* PieceFin = (*TileMap.Find(PosFin))->GetPiece();
	
	Spostato Mossa{};

	
	if ((*TileMap.Find(PosFin))->GetTileOwner() != (*TileMap.Find(PosInit))->GetTileOwner())
	{
		//aggiorno array che tiene conto dei pezzi mangiati 

		if ((*TileMap.Find(PosFin))->GetTileOwner() == 1)
		{

			//aggiungo a pezzi mangiati il pezzo mangiato
			PezziNeriMangiati.Add(PieceFin);

			//prendo il pezzo e lo metto fuori scacchiera
			PieceFin->SetGridPosition(GloXC, GloYC);
			
			if (GloXC <  0)
			{
				GloYC = -4;
				GloXC = 7;
			}
			FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXC, GloYC);
			GloXC = GloXC - 1;
			Position.Z = 5;
			PieceFin->SetActorLocation(Position);




		}
		else if ((*TileMap.Find(PosFin))->GetTileOwner() == 0)
		{

			//prendo il pezzo e lo metto fuori scacchiera
			PieceFin->SetGridPosition(GloXG, GloYG);

			//aggiungo a pezzi mangiati il pezzo mangiato
			PezziBianchiMangiati.Add(PieceFin);

			if (GloXG < 0)
			{
				GloYG = 11;
				GloXG = 7;
			}


			FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXG, GloYG);
			Position.Z = 5;
			
			GloXG = GloXG - 1;
			PieceFin->SetActorLocation(Position);
		}
	}


	APiece* Piece = (*TileMap.Find(PosInit))->GetPiece();


	Piece->SetGridPosition(PosFin.X, PosFin.Y);
	FVector Position = AChessboard::GetRelativeLocationByXYPosition(PosFin.X, PosFin.Y);
	Position.Z = 5;
	Piece->SetActorLocation(Position);

	Mossa.PosInit = PosInit;
	Mossa.PosFin = PosFin;
	Mossa.Pezzo = Piece;

	// Aggiungi l'oggetto appena creato all'array Mosse
	Mosse.Add(Mossa);

	// sistemo attributi della tile iniziale e finale 
	(*TileMap.Find(PosFin))->Piece = Piece;
	(*TileMap.Find(PosInit))->Piece = nullptr;

	(*TileMap.Find(PosFin))->SetTileStatus(CurrentPlayer, ETileStatus::OCCUPIED);
	(*TileMap.Find(PosInit))->SetTileStatus(-1, ETileStatus::EMPTY);

	Piece->SetCurrentTile((*TileMap.Find(PosFin)));


	ResetLegalMoves();
}



void AChessboard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalize tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;

}

// Called when the game starts or when spawned
void AChessboard::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}


void AChessboard::ResetField()
{
	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY);
	}
	OnReset.Broadcast();

	//AChessboard* Chessboard = Cast<AChessboard>(GetWorld()->GetAuthGameMode());
	/*GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
	*/
}

void AChessboard::GenerateField()
{
	for (int32 x = 0; x < BoardSize; x++)
	{
		for (int32 y = 0; y < BoardSize; y++)
		{


			FVector Location = AChessboard::GetRelativeLocationByXYPosition(x, y);
			Location.Z = 0;
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);



			if ((x+y)%2==0)
			{
			Obj->StaticMeshComponent->SetMaterial(0, WhiteMaterial);

			}
			else
			{
			Obj->StaticMeshComponent->SetMaterial(0, BlackMaterial);
			}

			Obj->Piece = nullptr;

			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(x, y);
			TileArray.Add(Obj);
			TileMap.Add(FVector2D(x, y), Obj);

			if (x == 0 || x == 7 || x==1 || x==6 )
			{
				GeneratePiece(x, y);	
			}
			

			
		}
	}

}

void AChessboard::GeneratePiece(int32 x, int32 y)
{
	const float PawnScale = TileSize / 180.0f;
	//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
	FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
	Position.Z = 5;
	int32 BW = 0;
	APiece* PieceObj =nullptr;

	if (x == 0 && y == 4)
	{


		PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(50);
	}

	else if (x == 7 && y == 4)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/KingMat.KingMat")));


		 PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(50);

	}

	else if (x == 0 && y == 3)
	{

		PieceObj = GetWorld()->SpawnActor<AQueen>(AQueen::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(40);
	
	}
	else if (x == 7 && y == 3)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/QueenMat.QueenMat")));

		PieceObj = GetWorld()->SpawnActor<AQueen>(AQueen::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(40);
	}
	else if (x == 0 && (y == 1 || y == 6))
	{

		PieceObj = GetWorld()->SpawnActor<AHorse>(AHorse::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(20);
	}
	else if (x == 7 && (y == 1 || y == 6))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/HorseMat.HorseMat")));

		PieceObj = GetWorld()->SpawnActor<AHorse>(AHorse::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(20);
	}
	else if (x == 1)
	{

		PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(10);
	}
	else if (x == 6)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/PawnMat.PawnMat")));

		PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(10);
	}
	
	else if (x == 0 && (y == 0 || y == 7))
	{
		PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(20);
	}
	else if (x == 7 && (y == 0 || y == 7))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/RookMat.RookMat")));

		PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(20);
		}

	else if (x == 0 && (y == 2 || y == 5))
	{

		PieceObj = GetWorld()->SpawnActor<ABishop>(ABishop::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(30);
	}
	else if (x == 7 && (y == 2 || y == 5))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/BishopMat.BishopMat")));

		PieceObj = GetWorld()->SpawnActor<ABishop>(ABishop::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(30);
	}

	ATile* Tile = (*TileMap.Find(FVector2D(x, y)));
	PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
	PieceObj->SetGridPosition(x, y);
	PieceObj->SetCurrentTile(Tile);

	// Applica la rotazione dopo che l'oggetto � stato creato con successo
	FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
	PieceObj->AddActorLocalRotation(Rotation);


	(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(BW, ETileStatus::OCCUPIED);
	(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;

}

FVector2D AChessboard::GetTilePosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AChessboard::GetTileArray()
{
	return TileArray;
}

FVector AChessboard::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

FVector2D AChessboard::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}

inline bool AChessboard::IsValidPosition(const FVector2D Position) const
{
	return 0 <= Position[0] && Position[0] < BoardSize && 0 <= Position[1] && Position[1] < BoardSize;
}


ATile* AChessboard::GetTileAtPosition(const FVector2D & Position) const {
	// Itera attraverso tutte le tile nel GField
	for (const auto& Tile : TileArray) {
		// Controlla se la posizione della tile corrente corrisponde alla posizione data
		if (FVector2D(Tile->GetActorLocation()) == Position) {
			return Tile; // Restituisci la tile corrente
		}
	}

	return nullptr; // Se non � stata trovata una tile corrispondente, restituisci nullptr
}




