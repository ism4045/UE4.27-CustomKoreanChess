// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/GameModeBase.h"
#include "Utility/MACalculate.h"
#include "KoreanChessGameMode.generated.h"

UCLASS(minimalapi)
class AKoreanChessGameMode : public AGameModeBase
{
	GENERATED_BODY()
	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
public:
	AKoreanChessGameMode();

	void ReceiveMessage(EMessageType MessageType, class AKoreanChessPlayerController* Sender, TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<AActor> Target, EActionMessageType ActionType);
	UFUNCTION(BlueprintCallable)
	void CheckCurrentTurn(EActionInTurn CurrentAction, class AKCPiece* DeathPiece = nullptr);
	void SetPlayerControllersLocation();
	void PlaySequence(TWeakObjectPtr<class AKCPiece> Target);
	void DeclineAllControllerInput();
protected:
	UFUNCTION(BlueprintCallable)
	TArray<class APlayerController*> GetPlayerControllerArray();
	void MoveSelectedActor(TWeakObjectPtr<AActor> ClickedActor, FVector WorldPosition);
	void AttackSelectedActor(TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<class AKCPiece> Target);
	void ActiveSkillSelectedActor(TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<class AKCPiece> Target);
	void SpawnSelectedActor(TWeakObjectPtr<AActor> ClickedActor, FVector WorldPosition);

	void ProgressNextTurn();
	void SetReadyPlayer();
	void SetAcceptPlayer(EMessageType MessageType);
	void GameStart();

	UFUNCTION()
	void SendGameOverMessage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnLobby();
	void ReturnLobby_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnMenu();
	void ReturnMenu_Implementation();

	UFUNCTION()
	void ManageTime();

	UFUNCTION(BlueprintCallable)
	void ChangeLoading();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TSubclassOf<class APieceController> BP_PieceController;
	UPROPERTY(EditDefaultsOnly, Category = "SpawnManager")
	TSubclassOf<class AKCSpawnManager> BP_PieceSpawnManager;

	UPROPERTY()
	class APieceController* PieceController;
	UPROPERTY()
	class AKCSpawnManager* PieceSpawnManager;
	UPROPERTY()
	class ALevelSequenceActor* DeathLevelSequence;

	TWeakObjectPtr<AKoreanChessPlayerController> MessageSender;
	TArray<TWeakObjectPtr<class AKoreanChessPlayerController>> ControllerArray;
	TArray<EActionInTurn> ActionInOneTurn;

	EPieceTeam WhoWin;
	FTimerHandle LimitTimeHandle;
};



