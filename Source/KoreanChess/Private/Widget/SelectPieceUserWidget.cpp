// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SelectPieceUserWidget.h"
#include "Components/Button.h"
#include "Character/KCPiece.h"
#include "PlayerController/KoreanChessPlayerController.h"
#include "GameState/KCGameStateBase.h"

void USelectPieceUserWidget::NativeOnInitialized()
{
	SkillButton->bIsEnabledDelegate.BindUFunction(this, "SetSkillButtonEnabled");
	AttackButton->bIsEnabledDelegate.BindUFunction(this, "SetAttackButtonEnabled");

	SkillButton->bIsEnabledDelegate.Execute();
	AttackButton->bIsEnabledDelegate.Execute();

	SkillButton->OnClicked.AddDynamic(this, &USelectPieceUserWidget::ClickedSkill);
	AttackButton->OnClicked.AddDynamic(this, &USelectPieceUserWidget::ClickedAttack);
}

bool USelectPieceUserWidget::SetSkillButtonEnabled()
{
	if (OwnerPiece == nullptr || OwnerPiece->GetSkillType() == ESkillType::PASSIVE) {
		return false;
	}
	if (Cast<AKCGameStateBase>(GetWorld()->GetGameState())->IsAlreadySkill())
		return false;
	return true;
}

bool USelectPieceUserWidget::SetAttackButtonEnabled()
{
	if (OwnerPiece == nullptr)
		return false;
	if (Cast<AKCGameStateBase>(GetWorld()->GetGameState())->IsAlreadyMove())
		return false;
	return true;
}

void USelectPieceUserWidget::ClickedAttack()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
		return;

	OwnerPiece->SelectButtonClicked();

	PlayerController->TrySendMessage(EMessageType::SELECT, Cast<AActor>(OwnerPiece),EActionMessageType::MOVEORATTACK);
}

void USelectPieceUserWidget::ClickedSkill()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
		return;
	OwnerPiece->SelectButtonClicked();

	PlayerController->TrySendMessage(EMessageType::SELECT, Cast<AActor>(OwnerPiece), EActionMessageType::ACTIVESKILL);
}
