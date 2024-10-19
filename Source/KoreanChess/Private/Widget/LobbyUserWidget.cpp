// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LobbyUserWidget.h"
#include "Widget/BaseMenuLobbyUserWidget.h"
#include "Widget/PlayerInfoUserWidget.h"
#include "GameInstance/KCGameInstance.h"
#include "PlayerController/LobbyPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/BorderSlot.h"
#include "Components/Button.h"

void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrPlayerNum = 0;
	ChangeOrderBtn->OnClicked.AddDynamic(this, &ULobbyUserWidget::ChangeOrder);
	Init();
}

void ULobbyUserWidget::NativeOnInitialized()
{
	ChangeOrderBtn->bIsEnabledDelegate.BindUFunction(this, TEXT("CanChangeOrder"));
	GameStartBtn->bIsEnabledDelegate.BindUFunction(this, TEXT("CanGameStart"));
}

void ULobbyUserWidget::Init()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	FString ServerName = GameInstance->GetServerName();
	Title->SetText(FText::FromString(ServerName));
	UpdateLobbyMember();
}

void ULobbyUserWidget::UpdateLobbyMember()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;

	TArray<FString> PlayerNames = GameInstance->GetPlayerNames();
	bool bIsFirstOrder = GameInstance->GetIsFirstOrder();
	if (CurrPlayerNum == PlayerNames.Num()) {
		for (int32 i = 0; i < PlayersInfo.Num(); i++) {
			if (PlayersInfo[i]->GetPlayerName() == GameInstance->GetMyName()) {
				PlayersInfo[i]->SetPlayerOrder(bIsFirstOrder);
			}
			else {
				PlayersInfo[i]->SetPlayerOrder(!bIsFirstOrder);
			}
		}
	}
	else if (CurrPlayerNum < PlayerNames.Num()) {
		for (int32 i = 0; i < PlayerNames.Num(); i++) {
			bool AlreadyExist = false;
			for (int32 j = 0; j < PlayersInfo.Num(); j++) {
				if (PlayerNames[i] == PlayersInfo[j]->GetPlayerName()) {
					AlreadyExist = true;
					break;
				}
			}
			if (AlreadyExist)
				continue;
			UPlayerInfoUserWidget* PlayerInfo = Cast<UPlayerInfoUserWidget>(CreateWidget(this, PlayerInfoWidget));
			PlayerInfo->SetPlayerName(PlayerNames[i]);
			if (PlayerNames[i] == GameInstance->GetMyName()) {
				PlayerInfo->SetPlayerOrder(bIsFirstOrder);
			}
			else {
				PlayerInfo->SetPlayerOrder(!bIsFirstOrder);
			}
			if (PlayerOneBorder->HasAnyChildren()) {
				UBorderSlot* PanelSlot = Cast<UBorderSlot>(PlayerTwoBorder->AddChild(PlayerInfo));
				PanelSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				PanelSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
			else {
				UBorderSlot* PanelSlot = Cast<UBorderSlot>(PlayerOneBorder->AddChild(PlayerInfo));
				PanelSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				PanelSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			}
			PlayersInfo.Add(PlayerInfo);
		}
	}
	else {
		for (int32 i = 0; i < PlayersInfo.Num(); i++) {
			FString PlayerName = PlayersInfo[i]->GetPlayerName();
			if (PlayerNames.FindByKey(PlayerName) == nullptr) {
				PlayersInfo[i]->RemoveFromParent();
			}
		}
	}
	CurrPlayerNum = PlayerNames.Num();
}

void ULobbyUserWidget::ChangeOrder()
{
	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
		return;
	PlayerController->ChangeOrder();
}

bool ULobbyUserWidget::CanChangeOrder()
{
	return GetWorld()->IsServer();
}

bool ULobbyUserWidget::CanGameStart()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return false;

	return GetWorld()->IsServer() && GameInstance->GetPlayerNames().Num() == 2;
}

void ULobbyUserWidget::DoReturn()
{

}
