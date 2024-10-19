// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UHPBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	friend class AKCPiece;
protected:
	void ReactDamage(float Percent);
	bool CheckReactingDamage();
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;
};
