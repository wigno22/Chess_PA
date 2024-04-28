// Fill out your copyright notice in the Description page of Project Settings.

//#include "BaseSign.h"
#include "Chessboard.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h" 
#include <Rook.h>
#include <Horse.h>
#include <Bishop.h>
#include <King.h>
#include <Queen.h>
#include <PawnPed.h> 
#include <ChessGameMode.h>
#include <HumanPlayer.h>
#include <RandomPlayer.h>
#include <GameMapsSettings.h>
#include <Components/TextBlock.h>
#include <Blueprint/UserWidget.h>
#include <BaseWidgetBlueprint.h>
#include <ChessBuild.h>
#include <ChessWidget.h>  



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

	UChessGameInstance* GameInstance = Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


}

bool AChessboard::InizializzaGioco()
{
	//cast di gamemode
	AChessGameMode* ChessGameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	//resetto il campo
	ResetField();

	ChessGameMode->IsGameOver = false;

	GenerateField();
	ClearScrollBox();
	GameMode->IsGameOver = false;	

	Cast<UChessGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetTurnMessage(TEXT(" Gioca ")); 
	return true;
}

bool  AChessboard::ColorLegalMoves(TArray<FVector2D> MosseLegali, ATile* TileDef)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GetWorld()->GetAuthGameMode());

	ATile* TileMangiato = nullptr;
	bool passa = false;

	Mangiata PedinaMangiata{};

	//ciclo per colorare le mosse legali
	for (int i=0; i<MosseLegali.Num(); i++)
	{
		int32 ProprietarioTile = (*TileMap.Find(MosseLegali[i]))->GetTileOwner();


		(*TileMap.Find(MosseLegali[i]))->StaticMeshComponent->SetMaterial(0, GreenMaterial);
		
		if (ProprietarioTile != -1 && ProprietarioTile != GameMode->CurrentPlayer)
		{
			TileMangiato= (*TileMap.Find(MosseLegali[i]));

			TileMangiato->StaticMeshComponent->SetMaterial(0, RedMaterial);
			
			
				PedinaMangiata.PosPezzoMangiante = TileDef->GetGridPosition();
				PedinaMangiata.PosPezzoMangiato = MosseLegali[i];
				PedinaMangiata.PezzoMangiante = TileDef->GetPiece();
				PedinaMangiata.PesoMangiante = TileDef->GetPiece()->GetWeight();
				PedinaMangiata.PezzoMangiato = TileMangiato->GetPiece();
				PedinaMangiata.PesoMangiato = TileMangiato->GetPiece()->GetWeight();
				PedinaMangiata.Player = GameMode->CurrentPlayer;

				//aggiorno array che tiene conto delle pedine mangiate
				Mangiate.push_back(PedinaMangiata);
						
				//ResetLegalMoves();
				passa = true;
			
		}

	}
	return passa;


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
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());
	ARandomPlayer* RandomPlayer = Cast<ARandomPlayer>(GetWorld()->GetAuthGameMode());
	APiece* PieceFin = (*TileMap.Find(PosFin))->GetPiece();

	
	//sono il compure e sposto fuorei se la posFinale è di 0
	if ((*TileMap.Find(PosFin))->GetTileOwner() == 0 && (*TileMap.Find(PosInit))->GetTileOwner() == 1 )
	{
		//prendo il pezzo e lo metto fuori scacchiera
		PieceFin->SetGridPosition(GloXG, GloYG);

		if (GloXG < 0)
		{
			GloYG = 12;
			GloXG = 7;
		}


		FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXG, GloYG);
		Position.Z = 5;

		PieceFin->SetActorLocation(Position);

		FVector2D Posizione = FVector2D(GloXG, GloYG);
		(*TileMap.Find(Posizione))->SetPiece(PieceFin);
		(*TileMap.Find(Posizione))->SetTileStatus(1, ETileStatus::OCCUPIED);

		RegistraMosse(PosFin, Posizione, PieceFin, 0);
		GloXG = GloXG - 1;


		(*TileMap.Find(PosFin))->Piece = nullptr;
		(*TileMap.Find(PosFin))->SetTileStatus(-1, ETileStatus::EMPTY);
	}

	//sono il compure e sposto fuorei se la posFinale è di 0
	if ((*TileMap.Find(PosFin))->GetTileOwner() == 1 && (*TileMap.Find(PosInit))->GetTileOwner() == 0)
	{
		//prendo il pezzo e lo metto fuori scacchiera
		PieceFin->SetGridPosition(GloXC, GloYC);

		if (GloXC < 0)
		{
			GloYC = 9;
			GloXC = 7;
		}
		FVector Position = AChessboard::GetRelativeLocationByXYPosition(GloXC, GloYC);
		Position.Z = 5;

		PieceFin->SetActorLocation(Position);
		//trasformo Position in fvector2d
		FVector2D Posizione = FVector2D(GloXC, GloYC);
		(*TileMap.Find(Posizione))->SetPiece(PieceFin);
		(*TileMap.Find(Posizione))->SetTileStatus(0, ETileStatus::OCCUPIED);
		RegistraMosse(PosFin, Posizione, PieceFin, 1);
		GloXC = GloXC - 1;


		(*TileMap.Find(PosFin))->Piece = nullptr;
		(*TileMap.Find(PosFin))->SetTileStatus(-1, ETileStatus::EMPTY);


	}




	APiece* Piece = (*TileMap.Find(PosInit))->GetPiece();
	Piece->SetGridPosition(PosFin.X, PosFin.Y);
	FVector Position = AChessboard::GetRelativeLocationByXYPosition(PosFin.X, PosFin.Y);
	Position.Z = 5;
	Piece->SetActorLocation(Position);

	GameMode->MoveCounter += 1;
	RegistraMosse(PosInit, PosFin, Piece, CurrentPlayer);
	
	
	
	//Promozione pedone
	//controllo se il pezzo è un pedone
	if ((*TileMap.Find(PosInit))->GetPiece()->GetTipoPedina() == "PEDONE")
	{
		if ((*TileMap.Find(PosInit))->GetTileOwner() == 1 && PosFin.X == 0)
		{
			
			Piece = PromozionePedReg(PosInit, PosFin, (*TileMap.Find(PosInit))->GetPiece());
			
		}
		else if ((*TileMap.Find(PosInit))->GetTileOwner() == 0 && PosFin.X == 7)
		{
			Piece = PromozionePedReg(PosInit, PosFin, (*TileMap.Find(PosInit))->GetPiece());
			
		}

	}
   


	// sistemo attributi della tile iniziale e finale 
	(*TileMap.Find(PosFin))->Piece = Piece;
	(*TileMap.Find(PosInit))->Piece = nullptr;

	(*TileMap.Find(PosFin))->SetTileStatus(CurrentPlayer, ETileStatus::OCCUPIED);
	(*TileMap.Find(PosInit))->SetTileStatus(-1, ETileStatus::EMPTY);

	Piece->SetCurrentTile((*TileMap.Find(PosFin)));

	
	
	ResetLegalMoves();

	
}

void AChessboard::DistruggiPezzo(FVector2D Position)
{
	(*TileMap.Find(Position))->GetPiece()->Destroy();
}

//funzione per eseguire la mossa, chiamata dopo aver controllato le validmoves
void AChessboard::ResetMossa(FVector2D PosInit, FVector2D PosFin, int32 CurrentPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Il pulsante è stato cliccato!"));
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	//pezzo nella posizione finale che dovrò spostare nella posizione iniziale
	APiece* PieceFin = (*TileMap.Find(PosFin))->GetPiece();

	//controllare le mosse che sono state mangiate (rimuovere dagli array delle pedine mangiate)

	//pezzo nella posizione iniziale che dovrò spostare nella posizione finale	
	APiece* Piece = (*TileMap.Find(PosInit))->GetPiece();

	//pezzo della finale lo sposto nell'iniziale
	Piece->SetGridPosition(PosFin.X, PosFin.Y);
	FVector Position = AChessboard::GetRelativeLocationByXYPosition(PosFin.X, PosFin.Y);
	Position.Z = 5;
	Piece->SetActorLocation(Position);
	FVector2D PosizioneTi;

	if (CurrentPlayer == 1 && GloXC <= 7 && PosInit.Y > 8)
	{
		GloXC = PosInit.X;
	}
	else if (CurrentPlayer == 0 && GloXG <= 7 && PosInit.Y > 8)
	{
		GloXG = PosInit.X;
	}
 	if (CurrentPlayer == 1 && GloXCQ <=7 && PosInit.Y > 12)
	{
		GloXCQ = PosInit.X;
	}
	else if (CurrentPlayer == 0 && GloXGQ <= 7 && PosInit.Y > 12)
	{
		GloXGQ = PosInit.X;
	}



	// sistemo attributi della tile iniziale e finale 
	(*TileMap.Find(PosFin))->Piece = Piece;
	(*TileMap.Find(PosInit))->Piece = nullptr;

	//questo non funziona se ho mangiato con la mossa
	(*TileMap.Find(PosFin))->SetTileStatus(CurrentPlayer, ETileStatus::OCCUPIED);
	(*TileMap.Find(PosInit))->SetTileStatus(-1, ETileStatus::EMPTY);

	Piece->SetCurrentTile((*TileMap.Find(PosFin)));





}

APiece* AChessboard::PromozionePedReg(FVector2D PosInit, FVector2D PosFin, APiece* Piece)
{
	//UChessBuild* ChessBuild = Cast<UChessBuild>(GetWorld()->GetAuthGameMode());
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	int32 x = PosFin.X;
	int32 y = PosFin.Y;
	FVector2D PosFinTemp = FVector2D(x, y);
	int32 OwnerPezzo = Piece->GetTile()->GetTileOwner();

	//DoMove(PosInit, PosFin, OwnerPezzo);

	if (OwnerPezzo == 1)
	{
		PosFin.X = GloXCQ;
		PosFin.Y = GloYCQ;
	}
	else
	{
		PosFin.X = GloXGQ;
		PosFin.Y = GloYGQ;
	}	

	DoMove(PosInit, PosFin, OwnerPezzo);
	GameMode->GField->Mosse[Mosse.Num() - 1].PosInit= GameMode->GField->Mosse[Mosse.Num() - 2].PosFin;
	//GameMode->GField->Mosse[Mosse.Num()-2].PosInit = PosInit;
	// aggiungo nella lista delle mosse la posizione -9,-9
	//RegistraMosse(PosInit, PosFin, (*TileMap.Find(PosInit))->GetPiece(), OwnerPezzo);

	

	PosFin.X = x;
	PosFin.Y = y;

	// inserisco regina del colore corretto
	const float PawnScale = TileSize / 180.0f;
	//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
	FVector Position = AChessboard::GetRelativeLocationByXYPosition(x, y);
	Position.Z = 5;
	std::string newName = "";
	int32 BW = 0;
	APiece* PieceObj = nullptr;
	 


	// sistemo attributi della tile iniziale e finale 
	
	(*TileMap.Find(PosInit))->Piece = nullptr;

	
	if (OwnerPezzo == 1)
	{
	
		BW = 1;
		PieceObj = CreaRegina(1, Position);
		newName = "REGINA_B" + std::to_string(static_cast<int>(FMath::RandRange(0, 10000)));
		
	}
	else {
		 
		PieceObj = CreaRegina(0, Position);
		newName = "REGINA_W" + std::to_string(static_cast<int>(FMath::RandRange(0, 10000)));
		
	}

	
	FString fString;
	PieceObj->SetName(fString.Printf(TEXT("%s"), *FString(newName.c_str())));
	
	PieceObj->SetTipoPedina("REGINA");
	ATile* Tile = (*TileMap.Find(FVector2D(x, y)));
	PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
	PieceObj->SetGridPosition(x, y);
	PieceObj->SetCurrentTile(Tile);

	// Applica la rotazione dopo che l'oggetto è stato creato con successo
	FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
	PieceObj->AddActorLocalRotation(Rotation);


	(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(BW, ETileStatus::OCCUPIED);
	(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;

	PieceObj->SetCurrentTile((*TileMap.Find(PosFin)));

	// aggiungo nella lista delle mosse la creazione della regina con posizione -9,-9 -> pos vera
	if (OwnerPezzo == 1)
	{
		PosInit.X = GloXCQ;
		GloXCQ--;
	}
	else
	{
		PosInit.X = GloXGQ;
		GloXGQ--;
	}

	
	PosInit.Y = GloYCQ;
	RegistraMosse(PosInit, PosFin, (*TileMap.Find(PosFin))->GetPiece(), OwnerPezzo);
	//RigeneraTileArray();

	return PieceObj;
}

void AChessboard::RegistraMosse(FVector2D PosInit, FVector2D PosFin, APiece* Piece, int32 PieceOwner)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	Spostato Mossa{};

	Mossa.PosInit = PosInit;
	Mossa.PosFin = PosFin;
	Mossa.Pezzo = Piece;
	Mossa.Player = PieceOwner;
	Mossa.NumeroMossa = GameMode->MoveCounter;

	// Aggiungi l'oggetto appena creato all'array Mosse
	Mosse.Add(Mossa);

	FString NomePezzo = Piece->GetName();
	
	bool Blocca = false;
	if (Mosse.Num() - 1 >= 0)
	{
		if (Mosse[Mosse.Num() - 1].PosFin.Y > 7)
		{
			Blocca = true;
		}
	}
	 
	
	GameMode->AddMossa(NomePezzo, PosFin, Mosse.Num(), Mossa.Player, Blocca);

}

// svuota la scroolbox
void AChessboard::ClearScrollBox()
{
	 GloXC = 7;
	 GloYC = 9;

	 GloXG = 7;
	 GloYG = 11;


	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	if ( Mosse.IsValidIndex(0))
	{
		while (Mosse.Num() > 0)
		{
			 Mosse.RemoveAt(0); // Rimuovi il primo elemento dall'array
		}
	}
	GameMode->HUD->ScrollBox->ClearChildren();

	 
}

// Funzione per calcolare la differenza di peso tra due oggetti della tua classe
int AChessboard::weightDifference(int32 PesoMangiante, int32 PesoMangiato)
{
	return PesoMangiante - PesoMangiato;
}

int AChessboard::partition(int low, int high)
{
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	int pivot = weightDifference(GameMode->GField->Mangiate[high].PesoMangiante, GameMode->GField->Mangiate[high].PesoMangiato);
	int i = low - 1;

	for (int j = low; j <= high - 1; j++) {
		if (weightDifference(GameMode->GField->Mangiate[j].PesoMangiante, GameMode->GField->Mangiate[j].PesoMangiato) < pivot) {
			i++;
			std::swap(GameMode->GField->Mangiate[i], GameMode->GField->Mangiate[j]);
		}
	}
	std::swap(GameMode->GField->Mangiate[i + 1], GameMode->GField->Mangiate[high]);
	return (i + 1);


}

// Funzione ricorsiva di ordinamento rapido
void AChessboard::quickSort(int low, int high) {
	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	if (low < high) {
		int pi = partition(low, high);
		quickSort(low, pi - 1);
		quickSort(pi + 1, high);
	}
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
		//voglio distruggere tutti gli oggetti
		if (Obj->GetPiece() != nullptr)
		{
			Obj->GetPiece()->Destroy();
		}
		Obj->Destroy();

	}

	for (int32 j = 7; j >= 0; j--)
	{
		for (int32 k = 9; k < 14; k++)
		{
			if ((*TileMap.Find(FVector2D(j, k)))->GetPiece() != nullptr)
			{
				(*TileMap.Find(FVector2D(j, k)))->Piece->Destroy();
			}
			
		}
	}

	ResetLegalMoves();
	TileArray.Empty();
	TileMap.Empty();
	Mosse.Empty();
	Mangiate.clear();
	 GloXC = 7;
	 GloYC = 9;

	 GloXG = 7;
	 GloYG = 11;

	

	OnReset.Broadcast();

	AChessGameMode* GameMode = Cast<AChessGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->IsGameOver = false;
	GameMode->MoveCounter = 0;
	GameMode->ChoosePlayerAndStartGame();
	
}

void AChessboard::GenerateField()
{

	//UChessBuild* ChessBuild2 = new UChessBuild();
	UChessBuild* Chessbuild3 = NewObject<UChessBuild>();

	
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

	for (int32 j = 7; j>=0 ; j--)
	{
		for (int32 k = 9; k < 15 ;k++)
		{
			FVector Location = AChessboard::GetRelativeLocationByXYPosition(j, k);
			Location.Z = 0;
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);

			Obj->Piece = nullptr;

			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetGridPosition(j, k);
			
			
			TileMap.Add(FVector2D(j, k), Obj);
		}
	}

	


}

FVector2D AChessboard::TrovaRe(int32 Prop)
{
	for (auto& CurrTile : TileMap)
	{
		APiece* Obj = CurrTile.Value->GetPiece();
		if(CurrTile.Value->GetTileOwner() == Prop)
		{ 
			if (Obj->GetTipoPedina() == "RE" )
			{
				return CurrTile.Key;
			}
		}
	}
	return FVector2D(-1, -1);

}

void AChessboard::RigeneraTileArray()
{
	TileArray.Empty();
	
	for (auto& CurrTile : TileMap)
	{
		ATile* Obj = CurrTile.Value;

		TileArray.Add(Obj);
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

ATile* AChessboard::GetTileAtPosition(const FVector2D & Position) const {
	FVector2D coordinate;  
	// Itera attraverso tutte le tile nel GField
	
	for(int32 i=0; i<TileArray.Num(); i++)
	{
		coordinate = FVector2D(TileArray[i]->GetGridPosition().X, TileArray[i]->GetGridPosition().Y);
		// Controlla se la posizione della tile corrente corrisponde alla posizione data
		UE_LOG(LogTemp, Warning, TEXT("coordinate %f %f"), coordinate.X, coordinate.Y);
		if (coordinate == Position)
		{
			return TileArray[i]; // Restituisci la tile corrente
		}
	}
	
	return nullptr; // Se non è stata trovata una tile corrispondente, restituisci nullptr
}

void AChessboard::GeneratePiece(int32 x, int32 y)
{
	
	const float PawnScale = TileSize / 180.0f;
	//salvo in location la posizione della pedina con GetRelativeLocationbyXYPosition
	FVector Position = GetRelativeLocationByXYPosition(x, y);
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
		UMaterial* NewMaterial = Cast<UMaterial>(
			StaticLoadObject(
				UMaterial::StaticClass(),
				NULL,
				TEXT("/Game/Materials/KingMat.KingMat")
			)
		);


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
	/*else if (x == 7 && y == 3)
	{
		PieceObj = CreaRegina(1, Position);
		BW = 1;

	}

	else if (x == 0 && (y == 1 || y == 6))
	{

		PieceObj = CreaCavallo(0, Position, y);
	}
	else if (x == 7 && (y == 1 || y == 6))
	{
		PieceObj = CreaCavallo(1, Position, y);
		BW = 1;
	}
	
	else if (x == 1)
	{

		PieceObj = CreaPedone(0, Position);
	}
	else if (x == 6)
	{
		PieceObj = CreaPedone(1, Position);
		BW = 1;
	}*/

	else if (x == 0 && (y == 0 || y == 7))
	{
		PieceObj = CreaTorre(0, Position, y);
	}
	
	/*else if (x == 7 && (y == 0 || y == 7))
	{
		PieceObj = CreaTorre(1, Position, y);
		BW = 1;
	}
	*/

	else if (x == 0 && (y == 2 || y == 5))
	{

		PieceObj = CreaAlfiere(0, Position, y);
	}
	/*
	else if (x == 7 && (y == 2 || y == 5))
	{
		PieceObj = CreaAlfiere(1, Position, y);
		BW = 1;
	}
	*/

	if (PieceObj != nullptr)
	{
	
	ATile* Tile = (*TileMap.Find(FVector2D(x, y)));
	PieceObj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
	PieceObj->SetGridPosition(x, y);
	PieceObj->SetCurrentTile(Tile);

	// Applica la rotazione dopo che l'oggetto è stato creato con successo
	FRotator Rotation = FRotator(0, 90, 0); // 90 gradi lungo l'asse Z
	PieceObj->AddActorLocalRotation(Rotation);


	(*TileMap.Find(FVector2D(x, y)))->SetTileStatus(BW, ETileStatus::OCCUPIED);
	(*TileMap.Find(FVector2D(x, y)))->Piece = PieceObj;
	}
}

APiece* AChessboard::CreaCavallo(int32 Colore, FVector Position, int32 y)
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

APiece* AChessboard::CreaPedone(int32 Colore, FVector Position)
{
	
	APiece* PieceObj = nullptr;
	PieceObj = GetWorld()->SpawnActor<APawnPed>(APawnPed::StaticClass(), Position, FRotator::ZeroRotator);
	PieceObj->SetWeight(10);
	PieceObj->SetTipoPedina("PEDONE");
	FVector2d Posizione = GetXYPositionByRelativeLocation(Position);
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

APiece* AChessboard::CreaTorre(int32 Colore, FVector Position, int32 y)
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

APiece* AChessboard::CreaAlfiere(int32 Colore, FVector Position, int32 y)
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

APiece* AChessboard::CreaRegina(int32 Colore, FVector Position)
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


 



