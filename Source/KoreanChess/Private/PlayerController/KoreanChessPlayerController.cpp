// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerController/KoreanChessPlayerController.h"
#include "GameMode/KoreanChessGameMode.h"
#include "Board/KCPointActor.h"
#include "Character/KCPiece.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameState/KCGameStateBase.h"
#include "HUD/KCHUD.h"
#include "Camera/CameraActor.h"
#include "GameInstance/KCGameInstance.h"

void AKoreanChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKoreanChessPlayerController, bIsFirstOrder);
}

void AKoreanChessPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bIsFirstOrder = Cast<UKCGameInstance>(GetWorld()->GetGameInstance())->GetIsFirstOrder() ? EPieceTeam::BLACK : EPieceTeam::WHITE;
}

AKoreanChessPlayerController::AKoreanChessPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
	CurrentMouseCursor = EMouseCursor::Default;
}

void AKoreanChessPlayerController::SetCurrentCursor(EMouseCursor::Type CursorType)
{
	CurrentMouseCursor = CursorType;
}

void AKoreanChessPlayerController::SendMessage_Implementation(EMessageType MessageType,AActor* ClickedActor, AActor* Target, EActionMessageType ActionType)
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	AKoreanChessGameMode* GameMode = Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode && GameState) {
		GameMode->ReceiveMessage(MessageType, this, ClickedActor, Target, ActionType);
	}
}

void AKoreanChessPlayerController::RecieveMessage_Implementation(EMessageType MessageType, EPieceTeam WhoWin, FVector CameraLocation, FRotator CameraRotation) {
	AKCHUD* HUD = Cast<AKCHUD>(MyHUD);
	AActor* Camera;
	switch (MessageType)
	{
	case EMessageType::GAMESTART:
		if (HUD) {
			HUD->StartGame();
		}
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		break;
	case EMessageType::SUCCESS:
		bEnableClickEvents = true;
		break;
	case EMessageType::GAMEOVER:
		if (HUD) {
			HUD->GameOver(WhoWin == bIsFirstOrder ? true : false);
		}
		break;
	case EMessageType::CANTINPUT:
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		break;
	case EMessageType::CINEMA:
		Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
		if (Camera) {
			Camera->SetActorRotation(CameraRotation);
			Camera->SetActorLocation(CameraLocation);
		}
		if (HUD) {
			HUD->Cinema();
		}
		break; 
	default:
		break;
	}
}

void AKoreanChessPlayerController::ExitServer_Implementation()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->GetMenuMapName()), true, "kicked");
}

void AKoreanChessPlayerController::TrySendMessage(EMessageType MessageType, AActor* ClickedActor, EActionMessageType ActionType)
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr || (MessageType != EMessageType::READY && MessageType != EMessageType::ACCEPT && MessageType != EMessageType::DECLINE && GameState->GetCurrentTurn() != bIsFirstOrder))
		return;
	switch (MessageType)
	{
	case EMessageType::SELECT:
		if (GameState->PieceSelect(Cast<AKCPiece>(ClickedActor), ActionType).IsEmpty()) {
			GameState->ResetMovableArray();
			ResetSelectedCharacter();
		}
		break;
	case EMessageType::MOVE:
	case EMessageType::SPAWN:
	case EMessageType::ATTACK:
		SendMessage(MessageType, SelectedCharacter.Get(), ClickedActor, ActionType);
		GameState->ResetMovableArray();
		ResetSelectedCharacter();
		bEnableClickEvents = false;
		break;
	case EMessageType::READY:
	case EMessageType::ACCEPT:
	case EMessageType::DECLINE:
	case EMessageType::TURNEND:
		SendMessage(MessageType, nullptr, nullptr, ActionType);
		break;
	}
}

void AKoreanChessPlayerController::SelectPiece(TWeakObjectPtr<class AKCPiece> Piece)
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr || GameState->GetCurrentTurn() != bIsFirstOrder)
		return;

	if (Cast<AKCPiece>(Piece)->GetState() == EPieceState::DEAD) {
		int32 CurrentScore = bIsFirstOrder == EPieceTeam::BLACK ? GameState->GetFirstOrderScore() : GameState->GetSecondOrderScore();
		int32 NeedScore = Cast<AKCPiece>(Piece)->GetDeathScore() * 3;
		if (CurrentScore < NeedScore) {
			ResetSelectedCharacter();
			GameState->ResetMovableArray();
			return;
		}
	}

	TWeakObjectPtr<AKCPiece> Temp = SelectedCharacter;
	ResetSelectedCharacter();
	GameState->ResetMovableArray();
	if (Temp == nullptr || Temp != Piece) {
		SelectedCharacter = Piece;
		SelectedCharacter->SelectThis();
		if (SelectedCharacter->GetState() == EPieceState::DEAD) {
			TrySendMessage(EMessageType::SELECT, Cast<AActor>(SelectedCharacter));
		}
	}
}

void AKoreanChessPlayerController::ChangeLoading_Implementation()
{
	Cast<AKCHUD>(GetHUD())->Loading();
}

void AKoreanChessPlayerController::ResetSelectedCharacter()
{
	if (SelectedCharacter != nullptr) {
		SelectedCharacter->DeSelectThis();
		SelectedCharacter.Reset();
	}
}
