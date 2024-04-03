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

void AChessboard::ColorLegalMoves(TArray<FVector2D> MosseLegali)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	
	//ciclo per colorare le mosse legali
	for (int i=0; i<MosseLegali.Num(); i++)
	{
		int32 ProprietarioTile = (*TileMap.Find(MosseLegali[i]))->GetTileOwner();

		(*TileMap.Find(MosseLegali[i]))->StaticMeshComponent->SetMaterial(0, GreenMaterial);
		
		if (ProprietarioTile != -1 && ProprietarioTile != GameMode->CurrentPlayer)
		{
			(*TileMap.Find(MosseLegali[i]))->StaticMeshComponent->SetMaterial(0, RedMaterial);
		}

	}


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
void AChessboard:: DoMove(FVector2D PosInit, FVector2D PosFin)
{
	//prendo il pezzo nella posizione iniziale e lo sposto nella posizione finale
	//poi metto la posizione iniziale a vuota e la posizione finale occupata


	APiece* PieceFin = (*TileMap.Find(PosFin))->GetPiece();
	 


	if ((*TileMap.Find(PosFin))->GetTileOwner() != (*TileMap.Find(PosInit))->GetTileOwner())
	{
		if ((*TileMap.Find(PosFin))->GetTileOwner() == 1)
		{
			
			PieceFin->SetGridPosition(GloXC, -3 );
			

			FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXC, -3);
			GloXC = GloXC - 1;
			Position.Z = 5;
			PieceFin->SetActorLocation(Position);



		}
		else if ((*TileMap.Find(PosFin))->GetTileOwner() == 0)
		{
			
			PieceFin->SetGridPosition(GloXG, +10);
			


			FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXG, +10);
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

	// sistemo attributi della tile iniziale e finale 
	(*TileMap.Find(PosFin))->Piece = Piece;
	(*TileMap.Find(PosInit))->Piece = nullptr;

	(*TileMap.Find(PosFin))->SetTileStatus(0, ETileStatus::OCCUPIED);
	(*TileMap.Find(PosInit))->SetTileStatus(-1, ETileStatus::EMPTY);

	

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

	if (x == 0 && y == 4)
	{
		
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);

		
		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);


		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}

	else if (x == 7 && y == 4)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/KingMat.KingMat")));
		
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D (x,y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;

	}

	else if (x == 0 && y == 3)
	{
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AQueen>(AQueen::StaticClass(), Position, FRotator::ZeroRotator);
	
		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 7 && y == 3)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/QueenMat.QueenMat")));

		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AQueen>(AQueen::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 0 && (y == 1 || y == 6))
	{
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AHorse>(AHorse::StaticClass(), Position, FRotator::ZeroRotator);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 7 && (y == 1 || y == 6))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/HorseMat.HorseMat")));

		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<AHorse>(AHorse::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 1)
	{
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 6)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/PawnMat.PawnMat")));

		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	
	else if (x == 0 && (y == 0 || y == 7))
	{
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 7 && (y == 0 || y == 7))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/RookMat.RookMat")));

		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
		}

	else if (x == 0 && (y == 2 || y == 5))
	{
		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<ABishop>(ABishop::StaticClass(), Position, FRotator::ZeroRotator);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(0, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
	else if (x == 7 && (y == 2 || y == 5))
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/BishopMat.BishopMat")));

		//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
		Position.Z = 5;
		//FRotator Rotation = FRotator(0, 90, 0);
		APiece* PieceObj = GetWorld()->SpawnActor<ABishop>(ABishop::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);

		PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		PieceObj->SetGridPosition(x, y);

		// Applica la rotazione dopo che l'oggetto è stato creato con successo
		FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
		PieceObj->AddActorLocalRotation(Rotation);

		(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(1, ETileStatus::OCCUPIED);
		(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
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

/*TArray<int32> AChessboard::GetLine(const FVector2D Begin, const FVector2D End) const
{
	int32 xSign;
	if (Begin[0] == End[0])
	{
		xSign = 0;
	}
	else
	{
		xSign = Begin[0] < End[0] ? 1 : -1;
	}

	int32 ySign;
	if (Begin[1] == End[1])
	{
		ySign = 0;
	}
	else
	{
		ySign = Begin[1] < End[1] ? 1 : -1;
	}

	TArray<int32> Line;
	int32 x = Begin[0] - xSign;
	int32 y = Begin[1] - ySign;
	do
	{
		x += xSign;
		y += ySign;
		//Line.Add((TileMap[FVector2D(x, y)])->GetOwner());
	} while (x != End[0] || y != End[1]);

	return Line;
}

 Called every frame
void AChessboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

