// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseMenuLobbyUserWidget.h"
#include "GameInstance/KCGameInstance.h"
#include "HUD/KCHUD.h"
#include "Components/Button.h"

void UBaseMenuLobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ReturnBtn->OnClicked.AddDynamic(this, &UBaseMenuLobbyUserWidget::DoReturn);
}

void UBaseMenuLobbyUserWidget::DoReturn()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;

	AKCHUD* HUD = Cast<AKCHUD>(GetOwningPlayer()->GetHUD());
	if (HUD == nullptr)
		return;

	HUD->ChangeWidget(GameInstance->GetMenuWidget());
}
