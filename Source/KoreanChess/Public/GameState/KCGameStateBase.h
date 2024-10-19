// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/GameStateBase.h"
#include "Utility/MACalculate.h"
#include "KCGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AKCGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:
	int32 GetFirstOrderScore() { return FirstOrderScore; }
	int32 GetSecondOrderScore() { return SecondOrderScore; }
	void IncreaseScore(int32 Value);
	void DecreaseScore(int32 Value);
	TArray<bool> GetReadyArray() { return ReadyArray; }
	void SetReadyArray(TArray<bool> NewReadyArray) { ReadyArray = NewReadyArray; }
	TArray<bool> GetAcceptArray() { return IsAcceptArray; }
	void SetAcceptArray(TArray<bool> NewAcceptArray) { IsAcceptArray = NewAcceptArray; }
	void SetAlreadyMove(bool NewMove) { bIsAlreadyMove = NewMove; }
	void SetAlreadySkill(bool NewSkill) { bIsAlreadySkill = NewSkill; }
	void SetDeathBoard(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> NewBlackBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> NewWhiteBoard);

	virtual void PostInitializeComponents() override;

	void ChangeTurn();
	EPieceTeam GetCurrentTurn() { return CurrentTurn; }
	int32 GetLimitTime() { return LimitTime; }
	FMAResultData PieceSelect(TWeakObjectPtr<class AKCPiece> ClickedActor, EActionMessageType ActionType);

	void ResetMovableArray();
	void UpdateMovableArray(TWeakObjectPtr<class AKCPiece> ClickedActor, EActionMessageType ActionType);

	FPointMap GetPointMap() { return PointMap; }
	void SetPointMap(FPointMap NewPointMap) { PointMap = NewPointMap; }
	
	void ResetLimitTime();
	void DecreaseTime() { LimitTime--; }

	bool IsAlreadyMove() { return bIsAlreadyMove; }
	bool IsAlreadySkill() { return bIsAlreadySkill; }
private:
	UPROPERTY(Replicated)
	TArray<bool> ReadyArray;
	UPROPERTY(Replicated)
	TArray<bool> IsAcceptArray;
	UPROPERTY(Replicated)
	bool bIsAlreadyMove;
	UPROPERTY(Replicated)
	bool bIsAlreadySkill;
	UPROPERTY(Replicated)
	EPieceTeam CurrentTurn = EPieceTeam::BLACK;
	UPROPERTY(Replicated)
	FPointMap PointMap;
	UPROPERTY(Replicated)
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard;
	UPROPERTY(Replicated)
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard;

	int32 DefaultTime;
	UPROPERTY(Replicated)
	int32 LimitTime;
	UPROPERTY(Replicated)
	int32 FirstOrderScore;
	UPROPERTY(Replicated)
	int32 SecondOrderScore;

	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCPointActor>> CanMovePointArray;
	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCPiece>> CanAttackActorArray;

	TUniquePtr<class MACalculate> Calculator;
};
