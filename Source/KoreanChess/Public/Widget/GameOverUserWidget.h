// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	UFUNCTION()
	void ApprovedRestart();
	UFUNCTION()
	void RejectRestart();
	UFUNCTION()
	FLinearColor SetAcceptReact1();
	UFUNCTION()
	FLinearColor SetAcceptReact2();

	void SetResultText(bool bIsWin);
private:
	UPROPERTY(EditAnywhere)
	FText WinText;
	UPROPERTY(EditAnywhere)
	FText LoseText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
	UPROPERTY(meta = (BindWidget))
	class UButton* ApprovedButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* RejectButton;
	UPROPERTY(meta = (BindWidget))
	class UImage* AcceptReact1;
	UPROPERTY(meta = (BindWidget))
	class UImage* AcceptReact2;
};
