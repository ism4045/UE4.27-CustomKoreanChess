// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DescriptionUserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "GameState/KCGameStateBase.h"
#include "PlayerController/KoreanChessPlayerController.h"
#include "Components/CircularThrobber.h"

void UDescriptionUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AcceptReact1->ColorAndOpacityDelegate.BindUFunction(this, "SetAcceptReact1");
	AcceptReact1->SynchronizeProperties();
	AcceptReact2->ColorAndOpacityDelegate.BindUFunction(this, "SetAcceptReact2");
	AcceptReact2->SynchronizeProperties();
	Ready->OnClicked.AddDynamic(this, &UDescriptionUserWidget::ClickedReadyButton);
}

FLinearColor UDescriptionUserWidget::SetAcceptReact1()
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	TArray<bool> ReadyArray = GameState->GetReadyArray();
	if (ReadyArray[0])
		return FLinearColor(0.0f, 0.0f, 1.0f);
	return FLinearColor(1.0f, 1.0f, 1.0f);
}

FLinearColor UDescriptionUserWidget::SetAcceptReact2()
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FLinearColor(1.0f, 1.0f, 1.0f);
	TArray<bool> ReadyArray = GameState->GetReadyArray();
	if (ReadyArray[1])
		return FLinearColor(0.0f, 0.0f, 1.0f);
	return FLinearColor(1.0f, 1.0f, 1.0f);
}

void UDescriptionUserWidget::ClickedReadyButton() {
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetOwningPlayer());
	if (PlayerController) {
		PlayerController->TrySendMessage(EMessageType::READY, nullptr);
	}
	Ready->SetVisibility(ESlateVisibility::Hidden);
	WaitCircle->SetVisibility(ESlateVisibility::HitTestInvisible);
}