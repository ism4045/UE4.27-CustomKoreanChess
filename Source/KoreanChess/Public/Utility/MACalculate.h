// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/KCData.h"
/**
 * 
 */
class KOREANCHESS_API MACalculate
{
public:
	MACalculate();
	FMAResultData GetResult(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard, FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter, EActionMessageType ActionType);
protected:
	FMAResultData GetResult_MaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_ChaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_JolLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_SangLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_PoLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_JangSaLogic(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_DeathPiece(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);

	FMAResultData GetResult_SaActiveSkill(FPointMap Table, TWeakObjectPtr<class AKCPiece> SelectedCharacter);
	FMAResultData GetResult_JangActiveSkill(TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard, TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard, TWeakObjectPtr<class AKCPiece> SelectedCharacter);

	void MakeCastleGraph();
	bool IsInCastle(TWeakObjectPtr<class AKCPiece> SelectedCharacter, FPos CurrentPos);
private:
	TMap<FPos, TArray<FPos>> CastleGraph;
};
