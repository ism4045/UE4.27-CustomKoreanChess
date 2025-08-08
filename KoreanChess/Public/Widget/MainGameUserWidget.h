// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Blueprint/UserWidget.h"
#include "MainGameUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UMainGameUserWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeOnInitialized() override;
public:
	void BindSelectedCharacter(TWeakObjectPtr<class AKCPiece> NewSelectedCharacter);
	void UnBindSelectedCharacter();
protected:
	UFUNCTION()
	FText SetMaxHPText();
	UFUNCTION()
	FText SetCurrentHPText();
	UFUNCTION()
	FText SetDamageText();
	UFUNCTION()
	FText SetResurrectionText();
	UFUNCTION()
	FText SetSkillTypeText();
	UFUNCTION()
	float SetHPBar();
	UFUNCTION()
	void ChangePieceStatusUI();

	UFUNCTION()
	FText SetLimitTimeText();
	UFUNCTION()
	FText SetCurrentFirstOrderText();

	void SetOpponentNameText();
	void SetOpponentFirstOrderText();
	UFUNCTION()
	FText SetOpponentScoreText();

	void SetMyNameText();
	void SetMyFirstOrderText();
	UFUNCTION()
	FText SetMyScoreText();

	UFUNCTION()
	bool SetTurnEndButtonEnabled();
	UFUNCTION()
	void ChangeNextTurn();
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHPText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentHPText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillTypeText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DescriptionText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DirectionText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PieceNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResurrectionText;
	UPROPERTY(meta = (BindWidget))
	class UOverlay* CharacterStatus;
	UPROPERTY(meta = (BindWidget))
	class UImage* Character_RenderTarget;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LimitTimeText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentFirstOrderText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OpponentNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OpponentFirstOrderText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OpponentScoreText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyNameText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyFirstOrderText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyScoreText;

	UPROPERTY(meta = (BindWidget))
	class UButton* TurnEndButton;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Appear;

	UPROPERTY()
	TWeakObjectPtr<class AKCPiece> SelectedCharacter;


	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> BlackMatArray;
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> WhilteMatArray;
	UPROPERTY(EditAnywhere)
	TArray<FText> PieceNameArray;
	UPROPERTY(EditAnywhere)
	TArray<FText> ResurrectionTextArray;
	UPROPERTY(EditAnywhere)
	TArray<FText> DirectionArray;
	UPROPERTY(EditAnywhere)
	TArray<FText> SkillDescriptionArray;
	UPROPERTY(EditAnywhere)
	FText ActiveKorean;
	UPROPERTY(EditAnywhere)
	FText PassiveKorean;
	UPROPERTY(EditAnywhere)
	FText FirstOrderKorean;
	UPROPERTY(EditAnywhere)
	FText SecondOrderKorean;
};
