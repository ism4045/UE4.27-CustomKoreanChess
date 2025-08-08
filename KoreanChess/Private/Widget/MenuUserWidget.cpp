// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MenuUserWidget.h"
#include "GameInstance/KCGameInstance.h"
#include "HUD/KCHUD.h"
#include "Components/Button.h"
void UMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(StartAnim);
	SearchBtn->OnClicked.AddDynamic(this, &UMenuUserWidget::GoToServerBrowser);
	HostBtn->OnClicked.AddDynamic(this, &UMenuUserWidget::GoToCreateServer);
}

void UMenuUserWidget::GoToServerBrowser()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	
	AKCHUD* HUD = Cast<AKCHUD>(GetOwningPlayer()->GetHUD());
	if (HUD == nullptr)
		return;

	HUD->ChangeWidget(GameInstance->GetServerBrowserWidget());
}

void UMenuUserWidget::GoToCreateServer()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;

	AKCHUD* HUD = Cast<AKCHUD>(GetOwningPlayer()->GetHUD());
	if (HUD == nullptr)
		return;

	HUD->ChangeWidget(GameInstance->GetServerSettingWidget());
}
