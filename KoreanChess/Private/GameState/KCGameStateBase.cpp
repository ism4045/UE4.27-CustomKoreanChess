// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/KCGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Character/KCPiece.h"
#include "Board/KCPointActor.h"
#include "PlayerController/KoreanChessPlayerController.h"

void AKCGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKCGameStateBase, CurrentTurn);
	DOREPLIFETIME(AKCGameStateBase, PointMap);
	DOREPLIFETIME(AKCGameStateBase, LimitTime);
	DOREPLIFETIME(AKCGameStateBase, ReadyArray);
	DOREPLIFETIME(AKCGameStateBase, IsAcceptArray);
	DOREPLIFETIME(AKCGameStateBase, FirstOrderScore);
	DOREPLIFETIME(AKCGameStateBase, SecondOrderScore);
	DOREPLIFETIME(AKCGameStateBase, bIsAlreadyMove);
	DOREPLIFETIME(AKCGameStateBase, bIsAlreadySkill);
	DOREPLIFETIME(AKCGameStateBase, BlackDeathBoard);
	DOREPLIFETIME(AKCGameStateBase, WhiteDeathBoard);
}

void AKCGameStateBase::IncreaseScore(int32 Value)
{
	if (CurrentTurn == EPieceTeam::BLACK) {
		FirstOrderScore += Value;
	}
	else {
		SecondOrderScore += Value;
	}
}

void AKCGameStateBase::DecreaseScore(int32 Value)
{
	if (CurrentTurn == EPieceTeam::BLACK) {
		FirstOrderScore -= Value;
	}
	else {
		SecondOrderScore -= Value;
	}
}

void AKCGameStateBase::SetDeathBoard(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> NewBlackBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> NewWhiteBoard)
{
	BlackDeathBoard = NewBlackBoard;
	WhiteDeathBoard = NewWhiteBoard;
}

void AKCGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Calculator = TUniquePtr<MACalculate>(new MACalculate());
	DefaultTime = 120;
	FirstOrderScore = 0;
	SecondOrderScore = 0;
	LimitTime = DefaultTime;
	ReadyArray = { false,false };
	bIsAlreadyMove = false;
	bIsAlreadySkill = false;
}

void AKCGameStateBase::ChangeTurn()
{
	bIsAlreadyMove = false;
	bIsAlreadySkill = false;
	if (CurrentTurn == EPieceTeam::BLACK) {
		CurrentTurn = EPieceTeam::WHITE;
	}
	else {
		CurrentTurn = EPieceTeam::BLACK;
	}
}

FMAResultData AKCGameStateBase::PieceSelect(TWeakObjectPtr<class AKCPiece> ClickedActor, EActionMessageType ActionType)
{
	FMAResultData Result = Calculator->GetResult(BlackDeathBoard, WhiteDeathBoard, PointMap, ClickedActor, ActionType);
	CanMovePointArray = Result.CanMovePointArray;
	CanAttackActorArray = Result.CanAttackActorArray;
	if (CanMovePointArray.Num() != 0 || CanAttackActorArray.Num() != 0) {
		UpdateMovableArray(ClickedActor, ActionType);
	}
	return Result;
}

void AKCGameStateBase::ResetMovableArray()
{
	for (int32 i = 0; i < CanMovePointArray.Num(); i++) {
		CanMovePointArray[i]->SetState(EPointState::NONE);
	}
	for (int32 i = 0; i < CanAttackActorArray.Num(); i++) {
		CanAttackActorArray[i]->SetState(EPieceState::IDLE);
	}
	CanMovePointArray.Empty();
	CanAttackActorArray.Empty();
}

void AKCGameStateBase::UpdateMovableArray(TWeakObjectPtr<class AKCPiece> ClickedActor, EActionMessageType ActionType)
{
	for (int32 i = 0; i < CanMovePointArray.Num(); i++) {
		if (ClickedActor->GetState() == EPieceState::DEAD) {
			CanMovePointArray[i]->SetState(EPointState::CANSPAWN);
		}
		else {
			CanMovePointArray[i]->SetState(EPointState::CANMOVE);
		}
	}
	for (int32 i = 0; i < CanAttackActorArray.Num(); i++) {
		if (ActionType == EActionMessageType::MOVEORATTACK) {
			CanAttackActorArray[i]->SetState(EPieceState::CANDAMAGED);
		}
		else {
			CanAttackActorArray[i]->SetState(EPieceState::CANSKILLDAMAGED);
		}
	}
}

void AKCGameStateBase::ResetLimitTime()
{
	LimitTime = DefaultTime;
}
