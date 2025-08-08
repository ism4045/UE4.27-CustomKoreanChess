// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UPlayerInfoUserWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class ULobbyUserWidget;
protected:
	FString GetPlayerName();
	bool GetPlayerOrder();
	void SetPlayerName(FString NewPlayerName);
	void SetPlayerOrder(bool bIsFirstOrder);
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerOrder;
};
