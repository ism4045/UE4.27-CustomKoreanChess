// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "BaseMenuLobbyUserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API ULobbyUserWidget : public UBaseMenuLobbyUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
public:
	void Init();
	void UpdateLobbyMember();
	UFUNCTION()
	void ChangeOrder();
	UFUNCTION()
	bool CanChangeOrder();
	UFUNCTION()
	bool CanGameStart();
protected:
	virtual void DoReturn() override;
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerInfoWidget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	class UBorder* PlayerOneBorder;
	UPROPERTY(meta = (BindWidget))
	class UBorder* PlayerTwoBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* ChangeOrderBtn;
	UPROPERTY(meta = (BindWidget))
	class UButton* GameStartBtn;

	UPROPERTY()
	TArray<class UPlayerInfoUserWidget*> PlayersInfo;
	int32 CurrPlayerNum;
};
