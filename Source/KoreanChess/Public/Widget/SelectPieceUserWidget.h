// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectPieceUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API USelectPieceUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	void SetOwnerPiece(TWeakObjectPtr<class AKCPiece> NewOwnerPiece) { OwnerPiece = NewOwnerPiece; }
	UFUNCTION()
	bool SetSkillButtonEnabled();
	UFUNCTION()
	bool SetAttackButtonEnabled();
	UFUNCTION()
	void ClickedAttack();
	UFUNCTION()
	void ClickedSkill();
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* AttackButton;
	UPROPERTY()
	TWeakObjectPtr<class AKCPiece> OwnerPiece;
};
