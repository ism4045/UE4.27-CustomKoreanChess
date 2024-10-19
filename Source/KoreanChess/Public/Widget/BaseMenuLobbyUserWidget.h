// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Blueprint/UserWidget.h"
#include "BaseMenuLobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UBaseMenuLobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	virtual void DoReturn();
	virtual void NativeConstruct() override;
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnBtn;
};
