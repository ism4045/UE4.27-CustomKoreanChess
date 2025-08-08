// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/PlayerController.h"
#include "KoreanChessPlayerController.generated.h"

UCLASS()
class AKoreanChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
public:
	void SetIsFirstOrder(EPieceTeam NewbIsFirstOrder) { bIsFirstOrder = NewbIsFirstOrder; }
	EPieceTeam GetIsFirstOrder() { return bIsFirstOrder; }
	AKoreanChessPlayerController();
	void SetCurrentCursor(EMouseCursor::Type CursorType);
	UFUNCTION(Server, Reliable)
	void SendMessage(EMessageType MessageType, AActor* ClickedActor, AActor* Target, EActionMessageType ActionType);
	void SendMessage_Implementation(EMessageType MessageType, AActor* ClickedActor, AActor* Target, EActionMessageType ActionType);
	UFUNCTION(Client, Reliable)
	void RecieveMessage(EMessageType MessageType, EPieceTeam WhoWin = EPieceTeam::BLACK, FVector CameraLocation = FVector(), FRotator CameraRotation = FRotator());
	void RecieveMessage_Implementation(EMessageType MessageType, EPieceTeam WhoWin = EPieceTeam::BLACK, FVector CameraLocation = FVector(), FRotator CameraRotation = FRotator());

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ExitServer();
	void ExitServer_Implementation();

	void TrySendMessage(EMessageType MessageType, AActor* ClickedActor, EActionMessageType ActionType = EActionMessageType::MOVEORATTACK);


	void SelectPiece(TWeakObjectPtr<class AKCPiece> Piece);
	UFUNCTION(Client, Reliable)
	void ChangeLoading();
	void ChangeLoading_Implementation();
protected:
	void ResetSelectedCharacter();

private:
	UPROPERTY(Replicated)
	EPieceTeam bIsFirstOrder;

	UPROPERTY()
	TWeakObjectPtr<class AKCPiece> SelectedCharacter;
};


