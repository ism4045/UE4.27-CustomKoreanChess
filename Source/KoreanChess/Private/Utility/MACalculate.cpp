// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/MACalculate.h"
#include "Character/KCPiece.h"
#include "Board/KCPointActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Board/KCDeathBoardActor.h"

MACalculate::MACalculate()
{
	MakeCastleGraph();
}

FMAResultData MACalculate::GetResult(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard, FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter, EActionMessageType ActionType)
{
	if (SelectedCharacter->GetState() == EPieceState::DEAD) {
		return GetResult_DeathPiece(Table, SelectedCharacter);
	}
	else {
		if (ActionType == EActionMessageType::MOVEORATTACK) {
			switch (SelectedCharacter->GetKind())
			{
			case EPieceKinds::JOL:
				return GetResult_JolLogic(Table, SelectedCharacter);
				break;
			case EPieceKinds::CHA:
				return GetResult_ChaLogic(Table, SelectedCharacter);
				break;
			case EPieceKinds::MA:
				return GetResult_MaLogic(Table, SelectedCharacter);
				break;
			case EPieceKinds::SANG:
				return GetResult_SangLogic(Table, SelectedCharacter);
			case EPieceKinds::PO:
				return GetResult_PoLogic(Table, SelectedCharacter);
			case EPieceKinds::SA:
			case EPieceKinds::JANG:
				return GetResult_JangSaLogic(Table, SelectedCharacter);
			default:
				break;
			}
		}
		else if (ActionType == EActionMessageType::ACTIVESKILL) {
			switch (SelectedCharacter->GetKind())
			{
			case EPieceKinds::SA:
				return GetResult_SaActiveSkill(Table, SelectedCharacter);
				break;
			case EPieceKinds::JANG:
				return GetResult_JangActiveSkill(BlackDeathBoard, WhiteDeathBoard, SelectedCharacter);
			default:
				break;
			}
		}
	}
	return FMAResultData();
}

FMAResultData MACalculate::GetResult_MaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	TArray<int32> DirX = { 0,1,-1 };
	TArray<int32> DirY = { -1,-2,-2 };

	for (int32 j = 0; j < 4; j++) {
		for (int32 i = 0; i < DirX.Num(); i++) {
			int32 NextX = CurrentPos.PosX + DirX[i];
			int32 NextY = CurrentPos.PosY + DirY[i];

			if (i == 0 && Table.Contains(FPos(NextX, NextY)) && Table[FPos(NextX, NextY)]->GetOnPiece() != nullptr) {
				break;
			}
			else if (i != 0) {
				if (Table.Contains(FPos(NextX, NextY))) {
					if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
						Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
					}
					else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
						Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
					}
				}
			}
		}
		for (int32 i = 0; i < DirX.Num(); i++) {
			int32 NextDirX = DirX[i] * 0 + DirY[i] * -1;
			int32 NextDirY = DirX[i] * 1 + DirY[i] * 0;
			DirX[i] = NextDirX;
			DirY[i] = NextDirY;
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_ChaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	TArray<int32> DirX = { -1,1,0,0 };
	TArray<int32> DirY = { 0,0,1,-1 };

	if (CastleGraph.Contains(CurrentPos)) {
		for (int32 i = 0; i < CastleGraph[CurrentPos].Num(); i++) {
			FPos NextDir = CastleGraph[CurrentPos][i] - CurrentPos;
			DirX.Add(NextDir.PosX);
			DirY.Add(NextDir.PosY);
		}
	}

	for (int32 i = 0; i < DirX.Num(); i++) {
		int32 TempX = CurrentPos.PosX;
		int32 TempY = CurrentPos.PosY;
		while (true)
		{
			int32 NextX = TempX + DirX[i];
			int32 NextY = TempY + DirY[i];
			if (Table.Contains(FPos(NextX, NextY))) {
				if (DirX[i] != 0 && DirY[i] != 0 && !IsInCastle(SelectedCharacter, FPos(NextX, NextY)))
					break;

				if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
					Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
				}
				else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
					Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
					break;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
			TempX = NextX;
			TempY = NextY;
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_JolLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	int32 JolDirY = SelectedCharacter.Get()->GetTeam() == EPieceTeam::BLACK ? 1 : -1;
	TArray<int32> DirX;
	TArray<int32> DirY;

	DirX = { -1,1,0 };
	DirY = { 0,0,JolDirY };

	if (CastleGraph.Contains(CurrentPos)) {
		for (int32 i = 0; i < CastleGraph[CurrentPos].Num(); i++) {
			FPos NextDir = CastleGraph[CurrentPos][i] - CurrentPos;
			if (NextDir.PosY == JolDirY) {
				DirX.Add(NextDir.PosX);
				DirY.Add(NextDir.PosY);
			}
		}
	}

	for (int32 i = 0; i < DirX.Num(); i++) {
		int32 NextX = CurrentPos.PosX + DirX[i];
		int32 NextY = CurrentPos.PosY + DirY[i];

		if (Table.Contains(FPos(NextX, NextY))) {
			if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
				Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
			}
			else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
				Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
			}
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_SangLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	TArray<int32> DirX = { 0,1,-1,2,-2 };
	TArray<int32> DirY = { -1,-2,-2,-3,-3 };

	for (int32 j = 0; j < 4; j++) {
		bool CanProcessToLeft = true;
		bool CanProcessToRight = true;
		for (int32 i = 0; i < DirX.Num(); i++) {
			int32 NextX = CurrentPos.PosX + DirX[i];
			int32 NextY = CurrentPos.PosY + DirY[i];
			if (i == 3 && !CanProcessToRight)
				continue;
			if (i == 4 && !CanProcessToLeft)
				continue;

			if (i == 0 && Table.Contains(FPos(NextX, NextY)) && Table[FPos(NextX, NextY)]->GetOnPiece() != nullptr) {
				break;
			}

			if (i == 1 || i == 2) {
				if (Table.Contains(FPos(NextX, NextY)) && Table[FPos(NextX, NextY)]->GetOnPiece() != nullptr) {
					if (i == 1) {
						CanProcessToRight = false;
					}
					else if (i == 2) {
						CanProcessToLeft = false;
					}
					continue;
				}
			}

			if (i ==3 || i == 4) {
				if (Table.Contains(FPos(NextX, NextY))) {
					if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
						Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
					}
					else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
						Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
					}
				}
			}
		}
		for (int32 i = 0; i < DirX.Num(); i++) {
			int32 NextDirX = DirX[i] * 0 + DirY[i] * -1;
			int32 NextDirY = DirX[i] * 1 + DirY[i] * 0;
			DirX[i] = NextDirX;
			DirY[i] = NextDirY;
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_PoLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	TArray<int32> DirX = { -1,1,0,0 };
	TArray<int32> DirY = { 0,0,1,-1 };
	if (CastleGraph.Contains(CurrentPos)) {
		for (int32 i = 0; i < CastleGraph[CurrentPos].Num(); i++) {
			FPos NextDir = CastleGraph[CurrentPos][i] - CurrentPos;
			DirX.Add(NextDir.PosX);
			DirY.Add(NextDir.PosY);
		}
	}

	for (int32 i = 0; i < DirX.Num(); i++) {
		int32 TempX = CurrentPos.PosX;
		int32 TempY = CurrentPos.PosY;
		FPos StartPos;
		bool IsFindWay = false;
		while (true)
		{
			int32 NextX = TempX + DirX[i];
			int32 NextY = TempY + DirY[i];
			if (Table.Contains(FPos(NextX, NextY))) {
				if (DirX[i] != 0 && DirY[i] != 0 && !IsInCastle(SelectedCharacter, FPos(NextX, NextY)))
					break;

				if (Table[FPos(NextX, NextY)]->GetOnPiece() != nullptr) {
					StartPos = FPos(NextX, NextY);
					if (Table[FPos(NextX, NextY)]->GetOnPiece()->GetKind() != EPieceKinds::PO)
						IsFindWay = true;
					break;
				}
			}
			else {
				break;
			}
			TempX = NextX;
			TempY = NextY;
		}
		while (IsFindWay)
		{
			int32 NextX = StartPos.PosX + DirX[i];
			int32 NextY = StartPos.PosY + DirY[i];
			if (Table.Contains(FPos(NextX, NextY))) {
				if (DirX[i] != 0 && DirY[i] != 0 && !IsInCastle(SelectedCharacter, FPos(NextX, NextY)))
					break;

				if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
					Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
				}
				else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
					if (Table[FPos(NextX, NextY)]->GetOnPiece()->GetKind() != EPieceKinds::PO) {
						Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
					}
					break;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
			StartPos = FPos(NextX,NextY);
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_JangSaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;

	FPos CurrentPos = SelectedCharacter.Get()->GetCurrentPos();
	FPos CastleCenter = SelectedCharacter->GetTeam() == EPieceTeam::BLACK ? FPos(4,1) : FPos(4,8);
	FPos ExtraDir = CastleCenter - CurrentPos;
	
	TArray<int32> DirX = { -1,1,0,0 };
	TArray<int32> DirY = { 0,0,1,-1 };

	if (ExtraDir.PosX != 0 && ExtraDir.PosY != 0 && UKismetMathLibrary::Abs(ExtraDir.PosX) == UKismetMathLibrary::Abs(ExtraDir.PosY)) {
		DirX.Add(ExtraDir.PosX);
		DirY.Add(ExtraDir.PosY);
	}
	else if (ExtraDir.PosX == 0 && ExtraDir.PosY == 0) {
		DirX.Add(1);
		DirY.Add(-1);
		DirX.Add(1);
		DirY.Add(1);
		DirX.Add(-1);
		DirY.Add(-1);
		DirX.Add(-1);
		DirY.Add(1);
	}

	for (int32 i = 0; i < DirX.Num(); i++) {
		int32 NextX = CurrentPos.PosX + DirX[i];
		int32 NextY = CurrentPos.PosY + DirY[i];
		if (IsInCastle(SelectedCharacter, FPos(NextX, NextY))) {
			if (Table[FPos(NextX, NextY)]->GetOnPiece() == nullptr) {
				Result.CanMovePointArray.Add(Table[FPos(NextX, NextY)]);
			}
			else if (Table[FPos(NextX, NextY)]->GetOnPiece().IsValid() && Table[FPos(NextX, NextY)]->GetOnPiece()->GetTeam() != SelectedCharacter->GetTeam()) {
				Result.CanAttackActorArray.Add(Table[FPos(NextX, NextY)]->GetOnPiece());
			}
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_DeathPiece(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;
	int32 StartY = SelectedCharacter->GetTeam() == EPieceTeam::BLACK ? 0 : 6;
	int32 EndY = SelectedCharacter->GetTeam() == EPieceTeam::BLACK ? 3 : 9;
	for (StartY; StartY <= EndY; StartY++) {
		for (int32 j = 0; j < 9; j++) {
			if (!Table[FPos(j, StartY)]->GetOnPiece().IsValid())
				Result.CanMovePointArray.Add(Table[FPos(j, StartY)]);
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_SaActiveSkill(FPointMap Table,TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;
	EPieceTeam Team = SelectedCharacter->GetTeam();
	for (int32 i = 0; i < 9; i++) {
		for (int32 j = 0; j < 10; j++) {
			if (Table.Contains(FPos(i, j))) {
				if (Table[FPos(i, j)]->GetOnPiece() != nullptr && Table[FPos(i, j)]->GetOnPiece()->GetTeam() != Team) {
					Result.CanAttackActorArray.Add(Table[FPos(i, j)]->GetOnPiece());
				}
			}
		}
	}
	return Result;
}

FMAResultData MACalculate::GetResult_JangActiveSkill(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard, TWeakObjectPtr<class AKCPiece> SelectedCharacter)
{
	FMAResultData Result;
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> FinalBoard = SelectedCharacter->GetTeam() == EPieceTeam::WHITE ? BlackDeathBoard : WhiteDeathBoard;
	for (int32 i = 0; i < FinalBoard.Num(); i++) {
		if (FinalBoard[i]->GetOnPiece() != nullptr) {
			Result.CanAttackActorArray.Add(FinalBoard[i]->GetOnPiece());
		}
	}
	return Result;
}

void MACalculate::MakeCastleGraph()
{
	TArray<FPos> Edges = { FPos(4,1) };
	CastleGraph.Add(FPos(3, 0), Edges);
	CastleGraph.Add(FPos(5, 0), Edges);
	CastleGraph.Add(FPos(3, 2), Edges);
	CastleGraph.Add(FPos(5, 2), Edges);
	Edges = { FPos(3,0),FPos(5,0),FPos(3,2),FPos(5,2) };
	CastleGraph.Add(FPos(4, 1), Edges);

	Edges = { FPos(4,8) };
	CastleGraph.Add(FPos(3, 7), Edges);
	CastleGraph.Add(FPos(5, 7), Edges);
	CastleGraph.Add(FPos(3, 9), Edges);
	CastleGraph.Add(FPos(5, 9), Edges);
	Edges = { FPos(3,7),FPos(5,7),FPos(3,9),FPos(5,9) };
	CastleGraph.Add(FPos(4, 8), Edges);
}

bool MACalculate::IsInCastle(TWeakObjectPtr<class AKCPiece> SelectedCharacter, FPos CurrentPos)
{
	if (SelectedCharacter->GetKind() == EPieceKinds::JANG || SelectedCharacter->GetKind() == EPieceKinds::SA) {
		switch (SelectedCharacter->GetTeam())
		{
		case EPieceTeam::BLACK:
			if (CurrentPos.PosX >= 3 && CurrentPos.PosX <= 5 && CurrentPos.PosY >= 0 && CurrentPos.PosY <= 2)
				return true;
			break;
		case EPieceTeam::WHITE:
			if (CurrentPos.PosX >= 3 && CurrentPos.PosX <= 5 && CurrentPos.PosY >= 7 && CurrentPos.PosY <= 9)
				return true;
			break;
		}
	}
	else if (SelectedCharacter->GetKind() == EPieceKinds::CHA || SelectedCharacter->GetKind() == EPieceKinds::JOL || SelectedCharacter->GetKind() == EPieceKinds::PO) {
		if (CurrentPos.PosX >= 3 && CurrentPos.PosX <= 5 && CurrentPos.PosY >= 0 && CurrentPos.PosY <= 2)
			return true;
		if (CurrentPos.PosX >= 3 && CurrentPos.PosX <= 5 && CurrentPos.PosY >= 7 && CurrentPos.PosY <= 9)
			return true;
	}
	return false;
}
