// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/GameOverUserWidget.h"
#include "GameState/KCGameStateBase.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PlayerController/KoreanChessPlayerController.h"
#include "Components/TextBlock.h"

void UGameOverUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AcceptReact1->ColorAndOpacityDelegate.BindUFunction(this, "SetAcceptReact1");
	AcceptReact1->SynchronizeProperties();
	AcceptReact2->ColorAndOpacityDelegate.BindUFunction(this, "SetAcceptReact2");
	AcceptReact2->SynchronizeProperties();

	ApprovedButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::ApprovedRestart);
	RejectButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::RejectRestart);
}

void UGameOverUserWidget::ApprovedRestart()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		PlayerController->TrySendMessage(EMessageType::ACCEPT, nullptr);
	}
	ApprovedButton->SetVisibility(ESlateVisibility::Hidden);
	RejectButton->SetVisibility(ESlateVisibility::Hidden);
}

void UGameOverUserWidget::RejectRestart()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		PlayerController->TrySendMessage(EMessageType::DECLINE, nullptr);
	}
	ApprovedButton->SetVisibility(ESlateVisibility::Hidden);
	RejectButton->SetVisibility(ESlateVisibility::Hidden);
}

FLinearColor UGameOverUserWidget::SetAcceptReact1()
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	TArray<bool> AcceptArray = GameState->GetAcceptArray();
	if (AcceptArray.Num() == 0)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	if (AcceptArray[0]) {
		return FLinearColor(0.0f, 0.0f, 1.0f);
	}
	else {
		return FLinearColor(1.0f, 0.0f, 0.0f);
	}
	return FLinearColor(1.0f, 1.0f, 1.0f);
}

FLinearColor UGameOverUserWidget::SetAcceptReact2()
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	TArray<bool> AcceptArray = GameState->GetAcceptArray();
	if (AcceptArray.Num() <= 1)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	if (AcceptArray[1]) {
		return FLinearColor(0.0f, 0.0f, 1.0f);
	}
	else {
		return FLinearColor(1.0f, 0.0f, 0.0f);
	}
	return FLinearColor(1.0f, 1.0f, 1.0f);
}

void UGameOverUserWidget::SetResultText(bool bIsWin)
{
	if (bIsWin)
		ResultText->SetText(WinText);
	else
		ResultText->SetText(LoseText);
}
