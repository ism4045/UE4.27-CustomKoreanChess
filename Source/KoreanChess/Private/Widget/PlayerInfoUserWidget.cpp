// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PlayerInfoUserWidget.h"
#include "Components/TextBlock.h"

FString UPlayerInfoUserWidget::GetPlayerName()
{
	return PlayerName->GetText().ToString();
}

bool UPlayerInfoUserWidget::GetPlayerOrder()
{
	bool Result = PlayerOrder->GetText().ToString() == "선공" ? true : false;
	return Result;
}

void UPlayerInfoUserWidget::SetPlayerName(FString NewPlayerName)
{
	PlayerName->SetText(FText::FromString(NewPlayerName));
}

void UPlayerInfoUserWidget::SetPlayerOrder(bool bIsFirstOrder)
{
	FString Str = bIsFirstOrder ? TEXT("선공") : TEXT("후공");
	PlayerOrder->SetText(FText::FromString(Str));
}
