// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/KCSpawnManager.h"
#include "Board/KCPointActor.h"
#include "Board/KCDeathBoardActor.h"
#include "Character/KCPiece.h"
#include "GameState/KCGameStateBase.h"

AKCSpawnManager::AKCSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
	bNetLoadOnClient = false;
}



void AKCSpawnManager::InitializePieces(FPointMap PointMap)
{
	int32 WhiteDefaultY = 6;
	int32 BlackDefaultY = 3;
	int32 DefaultX = 0;
	for (int i = 0; i < 5; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::JOL, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::JOL, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 2;
	}
	WhiteDefaultY = 9;
	BlackDefaultY = 0;
	DefaultX = 0;
	for (int i = 0; i < 2; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::CHA, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::CHA, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 8;
	}
	DefaultX = 1;
	for (int i = 0; i < 2; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::MA, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::MA, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 6;
	}
	DefaultX = 2;
	for (int i = 0; i < 2; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::SANG, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::SANG, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 4;
	}
	DefaultX = 3;
	for (int i = 0; i < 2; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::SA, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::SA, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 2;
	}

	WhiteDefaultY = 7;
	BlackDefaultY = 2;
	DefaultX = 1;
	for (int i = 0; i < 2; i++) {
		FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
		FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
		SpawnSpecificActor(EPieceKinds::PO, EPieceTeam::WHITE, SpawnLocationWhite);
		SpawnSpecificActor(EPieceKinds::PO, EPieceTeam::BLACK, SpawnLocationBlack);
		DefaultX += 6;
	}

	WhiteDefaultY = 8;
	BlackDefaultY = 1;
	DefaultX = 4;
	FVector SpawnLocationWhite = FVector(PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, WhiteDefaultY)]->GetActorLocation().Y, 110.0f);
	FVector SpawnLocationBlack = FVector(PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().X, PointMap[FPos(DefaultX, BlackDefaultY)]->GetActorLocation().Y, 110.0f);
	SpawnSpecificActor(EPieceKinds::JANG, EPieceTeam::WHITE, SpawnLocationWhite);
	SpawnSpecificActor(EPieceKinds::JANG, EPieceTeam::BLACK, SpawnLocationBlack);
}

AKCPiece* AKCSpawnManager::SpawnSpecificActor(EPieceKinds Kind, EPieceTeam Team, FVector SpawnLocation)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	switch (Kind)
	{
	case EPieceKinds::JOL:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteJol, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackJol, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::SA:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteSa, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackSa, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::SANG:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteSang, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackSang, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::MA:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteMa, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackMa, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::PO:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhitePo, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackPo, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::CHA:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteCha, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackCha, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	case EPieceKinds::JANG:
		switch (Team)
		{
		case EPieceTeam::WHITE:
			return GetWorld()->SpawnActor<AKCPiece>(BP_WhiteJang, SpawnLocation, FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			break;
		case EPieceTeam::BLACK:
			return GetWorld()->SpawnActor<AKCPiece>(BP_BlackJang, SpawnLocation, FRotator(0.0f, 180.0f, 0.0f), SpawnParameters);
			break;
		}
		break;
	}
	return nullptr;
}

void AKCSpawnManager::SpawnDeathPiece(EPieceKinds Kind, EPieceTeam Team)
{
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BoardArray;
	if (Team == EPieceTeam::BLACK)
		BoardArray = BlackDeathBoard;
	else
		BoardArray = WhiteDeathBoard;
	for (int32 i = 0; i < BoardArray.Num(); i++) {
		if (BoardArray[i]->IsEmpty()) {
			AKCPiece* Piece = SpawnSpecificActor(Kind, Team, BoardArray[i]->GetSpawnLocation());
			Piece->SetState(EPieceState::DEAD);
			break;
		}
	}
}


FPointMap AKCSpawnManager::InitializeBoard()
{
	FPointMap Result;
	float SpawnStartX = 935.0f;
	float SpawnStartY = -825.0f;
	for (int32 i = 0; i < 10; i++) {
		float CurrentY = SpawnStartY;
		for (int32 j = 0; j < 9; j++) {
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.bNoFail = true;
			SpawnParameters.Name = FName("Point" + FString::FromInt(j) + "_" + FString::FromInt(i));
			AKCPointActor* Actor = GetWorld()->SpawnActor<AKCPointActor>(BP_PointActor, FVector(SpawnStartX, CurrentY, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			Actor->Tags.Add(FName(FString::FromInt(j)));
			Actor->Tags.Add(FName(FString::FromInt(i)));
			Actor->SetPointInfo(FBoardPointInfo(j, i));
			Result.Add(FPos(j, i), Actor);
			CurrentY += 110.0f;
		}
		SpawnStartX -= 110.0f;
	}
	return Result;
}

void AKCSpawnManager::InitializeDeathBoard()
{
	float SpawnStartX = 880.0f;
	float SpawnStartY = -990.0f;
	for (int32 i = 0; i < 2; i++) {
		float CurrentX = SpawnStartX;
		for (int32 j = 0; j < 9; j++) {
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.bNoFail = true;
			SpawnParameters.Name = FName("DeathBoard" + FString::FromInt(i) + "_" + FString::FromInt(j));
			AKCDeathBoardActor* Actor = GetWorld()->SpawnActor<AKCDeathBoardActor>(BP_DeathBoardActor, FVector(CurrentX, SpawnStartY, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParameters);
			CurrentX -= 110.0f;
			if (i == 0) {
				BlackDeathBoard.Add(Actor);
			}
			else {
				WhiteDeathBoard.Add(Actor);
			}
		}
		SpawnStartY = 220.0f;
	}
	Cast<AKCGameStateBase>(GetWorld()->GetGameState())->SetDeathBoard(BlackDeathBoard, WhiteDeathBoard);
}
