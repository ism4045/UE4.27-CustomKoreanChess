// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Blueprint/UserWidget.h"
#include "DescriptionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UDescriptionUserWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeOnInitialized() override; 
	UFUNCTION()
	FLinearColor SetAcceptReact1();
	UFUNCTION()
	FLinearColor SetAcceptReact2();
	UFUNCTION()
	void ClickedReadyButton();
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* AcceptReact1;
	UPROPERTY(meta = (BindWidget))
	class UImage* AcceptReact2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Ready;
	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* WaitCircle;
};
