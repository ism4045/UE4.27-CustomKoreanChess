// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Blueprint/UserWidget.h"
#include "MenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
protected:
	UFUNCTION(BlueprintCallable)
	void GoToServerBrowser();
	UFUNCTION(BlueprintCallable)
	void GoToCreateServer();
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SearchBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBtn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* StartAnim;
};
