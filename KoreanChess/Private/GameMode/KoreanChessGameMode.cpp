// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/KoreanChessGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/KCPiece.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "PlayerController/PieceController.h"
#include "PlayerController/KoreanChessPlayerController.h"
#include "Board/KCPointActor.h"
#include "Utility/KCSpawnManager.h"
#include "GameState/KCGameStateBase.h"
#include "HUD/KCHUD.h"
#include "Board/KCDeathBoardActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Camera/CameraActor.h"
#include "GameInstance/KCGameInstance.h"

void AKoreanChessGameMode::StartPlay()
{
	Super::StartPlay();
	PieceController = GetWorld()->SpawnActor<APieceController>(BP_PieceController);
	PieceSpawnManager = GetWorld()->SpawnActor<AKCSpawnManager>(BP_PieceSpawnManager);
	FPointMap Result = PieceSpawnManager->InitializeBoard();
	PieceSpawnManager->InitializeDeathBoard();
	AKCGameStateBase* KCGameState = Cast<AKCGameStateBase>(GameState);
	KCGameState->SetPointMap(Result);
	DeathLevelSequence = Cast<ALevelSequenceActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelSequenceActor::StaticClass()));
}

void AKoreanChessGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(NewPlayer);
	if (PlayerController) {
		ControllerArray.Add(PlayerController);
		if (ControllerArray.Num() == 2) {
			FString P1Team = Cast<UKCGameInstance>(GetGameInstance())->GetIsFirstOrder() ? "BLACK" : "WHITE";
			FString P2Team = Cast<UKCGameInstance>(GetGameInstance())->GetIsFirstOrder() ? "WHITE" : "BLACK";
			AActor* PlayerStart = FindPlayerStart(Cast<AController>(ControllerArray[0]), P1Team);
			ControllerArray[0]->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
			PlayerStart = FindPlayerStart(Cast<AController>(ControllerArray[0]), P2Team);
			ControllerArray[1]->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
		}
	}
}

AKoreanChessGameMode::AKoreanChessGameMode()
{
	
}

TArray<class APlayerController*> AKoreanChessGameMode::GetPlayerControllerArray()
{
	TArray<class APlayerController*> Result;
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		Result.Add(Cast<APlayerController>(ControllerArray[i]));
	}
	return Result;
}

void AKoreanChessGameMode::MoveSelectedActor(TWeakObjectPtr<AActor> ClickedActor, FVector WorldPosition)
{
	if (ClickedActor.IsValid()) {
		PieceController->OnUnPossess();
		ActionInOneTurn.Add(EActionInTurn::MOVE);
		PieceController->OnPossess(Cast<APawn>(ClickedActor));
		PieceController->PieceMove(WorldPosition);
		Cast<AKCGameStateBase>(GameState)->SetAlreadyMove(true);
	}
}


void AKoreanChessGameMode::AttackSelectedActor(TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<class AKCPiece> Target)
{
	if (ClickedActor.IsValid()) {
		PieceController->OnUnPossess();

		ActionInOneTurn.Add(EActionInTurn::ATTACK);
		if (Cast<AKCPiece>(ClickedActor)->GetDamage() >= Target->GetHP()) {
			ActionInOneTurn.Add(EActionInTurn::DEAD);
			ActionInOneTurn.Add(EActionInTurn::MOVE);
		}
		PieceController->OnPossess(Cast<APawn>(ClickedActor));
		PieceController->AttackTarget(Target.Get());
		Cast<AKCGameStateBase>(GameState)->SetAlreadyMove(true);
	}
}

void AKoreanChessGameMode::ActiveSkillSelectedActor(TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<class AKCPiece> Target)
{
	if (ClickedActor.IsValid()) {
		PieceController->OnUnPossess();

		ActionInOneTurn.Add(EActionInTurn::ACTIVESKILL);
		PieceController->OnPossess(Cast<APawn>(ClickedActor));
		PieceController->DoActiveSkill(Target.Get());
		Cast<AKCGameStateBase>(GameState)->SetAlreadySkill(true);
	}
}

void AKoreanChessGameMode::SpawnSelectedActor(TWeakObjectPtr<AActor> ClickedActor, FVector WorldPosition)
{
	if (ClickedActor.IsValid()) {
		ClickedActor->SetActorLocation(FVector(WorldPosition.X,WorldPosition.Y,110.0f));
		Cast<AKCPiece>(ClickedActor)->SetState(EPieceState::IDLE);
		Cast<AKCGameStateBase>(GameState)->DecreaseScore(Cast<AKCPiece>(ClickedActor)->GetDeathScore() * 3);
	}
	MessageSender->RecieveMessage(EMessageType::SUCCESS);
}

void AKoreanChessGameMode::ProgressNextTurn()
{
	GetWorld()->GetTimerManager().ClearTimer(LimitTimeHandle);
	Cast<AKCGameStateBase>(GameState)->ChangeTurn();
	Cast<AKCGameStateBase>(GameState)->ResetLimitTime();
	GetWorld()->GetTimerManager().SetTimer(LimitTimeHandle, this, &AKoreanChessGameMode::ManageTime, 1.0f, true, 1.0f);
}


void AKoreanChessGameMode::ReceiveMessage(EMessageType MessageType, AKoreanChessPlayerController* Sender,TWeakObjectPtr<AActor> ClickedActor, TWeakObjectPtr<AActor> Target, EActionMessageType ActionType)
{
	MessageSender = Sender;
	switch (MessageType)
	{
	case EMessageType::SPAWN:
		SpawnSelectedActor(ClickedActor, Target->GetActorLocation());
		break;
	case EMessageType::MOVE:
		MoveSelectedActor(ClickedActor, Target.Get()->GetActorLocation());
		break;
	case EMessageType::ATTACK:
		if (ActionType == EActionMessageType::MOVEORATTACK) {
			AttackSelectedActor(ClickedActor, Cast<AKCPiece>(Target));
		}
		else {
			ActiveSkillSelectedActor(ClickedActor, Cast<AKCPiece>(Target));
		}
		break;
	case EMessageType::READY:
		SetReadyPlayer();
		break;
	case EMessageType::ACCEPT:
	case EMessageType::DECLINE:
		SetAcceptPlayer(MessageType);
		break;
	case EMessageType::TURNEND:
		ProgressNextTurn();
		break;
	default:
		break;
	}
}

void AKoreanChessGameMode::CheckCurrentTurn(EActionInTurn CurrentAction, AKCPiece* DeathPiece)
{
	ActionInOneTurn.Remove(CurrentAction);
	bool bIsGameOver = false;
	if (CurrentAction == EActionInTurn::DEAD && DeathPiece != nullptr && DeathPiece->GetState() != EPieceState::DEAD) {
		Cast<AKCGameStateBase>(GameState)->IncreaseScore(DeathPiece->GetDeathScore());
		if (DeathPiece->GetKind() == EPieceKinds::JOL || DeathPiece->GetKind() == EPieceKinds::SANG || DeathPiece->GetKind() == EPieceKinds::MA) {
			PieceSpawnManager->SpawnDeathPiece(DeathPiece->GetKind(), DeathPiece->GetTeam());
		}
		else if (DeathPiece->GetKind() == EPieceKinds::JANG) {
			bIsGameOver = true;
			GetWorld()->GetTimerManager().ClearTimer(LimitTimeHandle);
			WhoWin = DeathPiece->GetTeam() == EPieceTeam::BLACK ? EPieceTeam::WHITE : EPieceTeam::BLACK;
		}
	}
	if (ActionInOneTurn.Num() == 0 && !bIsGameOver) {
		MessageSender->RecieveMessage(EMessageType::SUCCESS);
	}
}

void AKoreanChessGameMode::SetPlayerControllersLocation()
{
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		if (ControllerArray[i]->GetIsFirstOrder() == EPieceTeam::BLACK) {
			AActor* PlayerStart = FindPlayerStart(Cast<AController>(ControllerArray[i]), "BLACK");
			ControllerArray[i]->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
			RestartPlayerAtPlayerStart(Cast<AController>(ControllerArray[i]), PlayerStart);
		}
		else {
			AActor* PlayerStart = FindPlayerStart(Cast<AController>(ControllerArray[i]), "WHITE");
			ControllerArray[i]->GetPawn()->SetActorTransform(PlayerStart->GetActorTransform());
			RestartPlayerAtPlayerStart(Cast<AController>(ControllerArray[i]), PlayerStart);
		}
	}
}

void AKoreanChessGameMode::PlaySequence(TWeakObjectPtr<class AKCPiece> Target)
{
	DeathLevelSequence->GetSequencePlayer()->Play();
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "SendGameOverMessage");
	DeathLevelSequence->GetSequencePlayer()->OnFinished.Add(Delegate);
	AActor* Camera = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	FVector CameraLocation = FVector();
	FRotator CameraRotation = FRotator();
	if (Camera) {
		if (Target->GetTeam() == EPieceTeam::BLACK) {
			CameraRotation = FRotator(-30.0f, 0.0f, 0.0f);
		}
		else {
			CameraRotation = FRotator(-30.0f, 180.0f, 0.0f);
		}
		CameraLocation = FVector(440.0f, Target->GetActorLocation().Y, 400.0f);
	}
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		ControllerArray[i]->RecieveMessage(EMessageType::CINEMA, WhoWin,CameraLocation, CameraRotation);
	}
}

void AKoreanChessGameMode::DeclineAllControllerInput()
{
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		ControllerArray[i]->RecieveMessage(EMessageType::CANTINPUT, WhoWin);
	}
}

void AKoreanChessGameMode::SetReadyPlayer()
{
	TArray<bool> ReadyArray = Cast<AKCGameStateBase>(GameState)->GetReadyArray();
	if (!ReadyArray[0]) {
		ReadyArray[0] = true;
	}
	else {
		ReadyArray[1] = true;
		GameStart();
	}
	Cast<AKCGameStateBase>(GameState)->SetReadyArray(ReadyArray);
}

void AKoreanChessGameMode::SetAcceptPlayer(EMessageType MessageType)
{
	TArray<bool> AcceptArray = Cast<AKCGameStateBase>(GameState)->GetAcceptArray();
	if (MessageType == EMessageType::ACCEPT) {
		AcceptArray.Add(true);
	}
	else {
		AcceptArray.Add(false);
	}
	if (AcceptArray.Num() == 2) {
		if (AcceptArray[0] && AcceptArray[1]) {
			ReturnLobby();
		}
		else {
			ReturnMenu();
		}
	}

	Cast<AKCGameStateBase>(GameState)->SetAcceptArray(AcceptArray);
}

void AKoreanChessGameMode::GameStart() {
	PieceSpawnManager->InitializePieces(Cast<AKCGameStateBase>(GameState)->GetPointMap());
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		ControllerArray[i]->RecieveMessage(EMessageType::GAMESTART);
	}
	GetWorld()->GetTimerManager().SetTimer(LimitTimeHandle, this, &AKoreanChessGameMode::ManageTime, 1.0f, true, 1.0f);
}

void AKoreanChessGameMode::SendGameOverMessage()
{
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		ControllerArray[i]->RecieveMessage(EMessageType::GAMEOVER, WhoWin);
	}
}

void AKoreanChessGameMode::ReturnLobby_Implementation()
{
}

void AKoreanChessGameMode::ReturnMenu_Implementation()
{
}

void AKoreanChessGameMode::ManageTime()
{
	Cast<AKCGameStateBase>(GameState)->DecreaseTime();
	if (Cast<AKCGameStateBase>(GameState)->GetLimitTime() < 0) {
		ProgressNextTurn();
	}
}

void AKoreanChessGameMode::ChangeLoading()
{
	for (int32 i = 0; i < ControllerArray.Num(); i++) {
		ControllerArray[i]->ChangeLoading();
	}
}
