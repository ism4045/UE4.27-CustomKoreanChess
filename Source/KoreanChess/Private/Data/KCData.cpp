// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/KCData.h"

FBoardPointInfo::FBoardPointInfo()
{
	Position.PosX = 0;
	Position.PosY = 0;
	State = EPointState::NONE;
}

FBoardPointInfo::FBoardPointInfo(FPos NewPosition, EPointState NewState)
{
	Position = NewPosition;
	State = NewState;
}

FBoardPointInfo::FBoardPointInfo(int64 NewPosX, int64 NewPosY, EPointState NewState)
{
	Position.PosX = NewPosX;
	Position.PosY = NewPosY;
	State = NewState;
}

void FBoardPointInfo::SetPosition(int64 NewPosX, int64 NewPosY)
{
	Position.PosX = NewPosX;
	Position.PosY = NewPosY;
}

void FBoardPointInfo::SetPosition(FPos NewPosition)
{
	Position = NewPosition;
}

void FBoardPointInfo::SetState(EPointState NewState)
{
	State = NewState;
}

FPointMap::FPointMap()
{
}

FPointMap::FPointMap(TArray<FPos> NewPosArr, TArray<TWeakObjectPtr<class AKCPointActor>> NewPieceArr)
{
	Pos = NewPosArr;
	Piece = NewPieceArr;
}

void FPointMap::Add(FPos NewPos, TWeakObjectPtr<class AKCPointActor> NewPiece)
{
	Pos.Add(NewPos);
	Piece.Add(NewPiece);
}

bool FPointMap::Contains(FPos CurrentPos)
{
	for (int i = 0; i < Pos.Num(); i++) {
		if (Pos[i] == CurrentPos)
			return true;
	}
	return false;
}

TWeakObjectPtr<class AKCPointActor> FPointMap::operator[](FPos Key)
{
	for (int i = 0; i < Pos.Num(); i++) {
		if (Pos[i] == Key) {
			return Piece[i];
		}
	}
	return nullptr;
}
