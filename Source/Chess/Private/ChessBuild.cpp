
// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessBuild.h"
#include <Rook.h>
#include <Horse.h>
#include <Bishop.h>
#include <King.h>
#include <Queen.h>
#include <PawnPed.h>
#include <string>
#include <Chessboard.h>


// Sets default values
UChessBuild::UChessBuild()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}



/*
void UChessBuild::GeneratePiece(int32 x, int32 y)
{
	

	//cast di chessboard
	AChessboard* Chessboard = Cast<AChessboard>(GetWorld()->GetFirstPlayerController()->GetPawn());


	const float PawnScale = Chessboard->TileSize / 180.0f;
	//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
	FVector Position = Chessboard->GetRelativeLocationByXYPosition(x, y);
	Position.Z = 5;
	int32 BW = 0;
	APiece* PieceObj = nullptr;

	if (x == 0 && y == 4)
	{
		PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);
		PieceObj->SetWeight(100);
		PieceObj->SetName("RE_W");
		PieceObj->SetTipoPedina("RE");
	}

	else if (x == 7 && y == 4)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/KingMat.KingMat")));


		PieceObj = GetWorld()->SpawnActor<AKing>(AKing::StaticClass(), Position, FRotator::ZeroRotator);
		//cambio colore per il re nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		BW = 1;
		PieceObj->SetWeight(100);
		PieceObj->SetName("RE_B");
		PieceObj->SetTipoPedina("RE");
	}

	else if (x == 0 && y == 3)
	{
		PieceObj = CreaRegina(0, Position);


	}
	else if (x == 7 && y == 3)
	{
		PieceObj = CreaRegina(1, Position);
		BW = 1;

	}

	else if (x == 0 && (y == 1 || y == 6))
	{

		CreaCavallo(0, Position, y);
	}
	else if (x == 7 && (y == 1 || y == 6))
	{
		CreaCavallo(1, Position, y);
			BW = 1;
	}
	else if (x == 1)
	{

		CreaPedone(0, Position);
	}
	else if (x == 6)
	{
		CreaPedone(1, Position);
		BW = 1;
	}

	else if (x == 0 && (y == 0 || y == 7))
	{
		CreaTorre(0, Position, y);
	}
	else if (x == 7 && (y == 0 || y == 7))
	{
		CreaTorre(1, Position, y);
		BW = 1;
	}


	else if (x == 0 && (y == 2 || y == 5))
	{

		CreaAlfiere(0, Position, y);
	}
	else if (x == 7 && (y == 2 || y == 5))
	{
		CreaAlfiere(1, Position, y);
		BW = 1;
	}


	


	ATile* Tile = (*Chessboard->TileMap.Find(FVector2D(x, y)));
	PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
	PieceObj->SetGridPosition(x, y);
	PieceObj->SetCurrentTile(Tile);

	// Applica la rotazione dopo che l'oggetto è stato creato con successo
	FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
	PieceObj->AddActorLocalRotation(Rotation);


	(*Chessboard->TileMap.Find(FVector2D(x, y)))->SetTileStatus(BW, ETileStatus::OCCUPIED);
	(*Chessboard->TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;

}


APiece* UChessBuild::CreaCavallo(int32 Colore, FVector Position, int32 y)
{
	APiece* PieceObj = nullptr;
	PieceObj = GetWorld()->SpawnActor<AHorse>(AHorse::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(40);
	PieceObj->SetTipoPedina("CAVALLO");

	if (Colore == 0) //bianco
	{

		if (y == 1)
		{
			PieceObj->SetName("CAVALLO_W1");
		}
		else
		{
			PieceObj->SetName("CAVALLO_W2");
		}


	}
	else //nero
	{

		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/HorseMat.HorseMat")));

		//cambio colore in nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		
		if (y == 1)
		{
			PieceObj->SetName("CAVALLO_B1");
		}
		else
		{
			PieceObj->SetName("CAVALLO_B2");
		}



	}

	return PieceObj;
}

APiece* UChessBuild::CreaPedone(int32 Colore, FVector Position)
{
	AChessboard* Chessboard = Cast<AChessboard>(GetWorld()->GetFirstPlayerController()->GetPawn());

	APiece* PieceObj = nullptr;
	PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(10);
	PieceObj->SetTipoPedina("PEDONE");
	FVector2d Posizione = Chessboard->GetXYPositionByRelativeLocation(Position);
	std::string newName = "";	
	if (Colore == 0) //bianco
	{
		newName = "PEDONE_W" + std::to_string(static_cast<int>(Posizione.Y));

	}
	else //nero
	{

		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/PawnMat.PawnMat")));
		//cambio colore in nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
	    newName = "PEDONE_B" + std::to_string(static_cast<int>(Position.Y));


	}
	// Converti std::string in FString usando FString::Printf
	FString fString;

	PieceObj->SetName(fString.Printf(TEXT("%s"), *FString(newName.c_str())));

	return PieceObj;
}

APiece* UChessBuild::CreaTorre(int32 Colore, FVector Position, int32 y)
{
	APiece* PieceObj = nullptr;
	PieceObj = GetWorld()->SpawnActor<ARook>(ARook::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(20);
	PieceObj->SetTipoPedina("TORRE");

	if (Colore == 0) //bianco
	{

		if (y == 0)
		{
			PieceObj->SetName("TORRE_W1");
		}
		else
		{
			PieceObj->SetName("TORRE_W2");
		}

	}
	else //nero
	{

		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/RookMat.RookMat")));

		//cambio colore per in nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		if (y == 0)
		{
			PieceObj->SetName("TORRE_B1");
		}
		else
		{
			PieceObj->SetName("TORRE_B2");
		}


	}

	return PieceObj;
}

APiece* UChessBuild::CreaAlfiere(int32 Colore, FVector Position, int32 y)
{
	APiece* PieceObj = nullptr;
	PieceObj = GetWorld()->SpawnActor<ABishop>(ABishop::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(30);
	PieceObj->SetTipoPedina("ALFIERE");

	if (Colore == 0) //bianco
	{
		if (y == 2)
		{
			PieceObj->SetName("ALFIERE_W1");
		}
		else
		{
			PieceObj->SetName("ALFIERE_W2");
		}


	}
	else //nero
	{

		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/BishopMat.BishopMat")));
		//cambio colore per in nero
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		

		if (y == 2)
		{
			PieceObj->SetName("ALFIERE_B1");
		}
		else
		{
			PieceObj->SetName("ALFIERE_B2");
		}

	}

	return PieceObj;
}

APiece* UChessBuild::CreaRegina(int32 Colore, FVector Position)
{
	APiece* PieceObj = GetWorld()->SpawnActor<AQueen>(AQueen::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(50);
	PieceObj->SetName("REGINA_W");
	PieceObj->SetTipoPedina("REGINA");

	if (Colore == 1)
	{
		UMaterial* NewMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, TEXT("/Game/Materials/QueenMat.QueenMat")));
		UStaticMeshComponent* MeshComponent = PieceObj->FindComponentByClass<UStaticMeshComponent>();
		MeshComponent->SetMaterial(0, NewMaterial);
		PieceObj->SetName("REGINA_B");
	}
	return PieceObj;
}

*/
