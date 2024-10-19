// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/HUD.h"
#include "KCHUD.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AKCHUD : public AHUD
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
public:
	void ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass);
	void UpdateLobbyMember();
	void BindPiece(TWeakObjectPtr<class AKCPiece> NewSelectedCharacter);
	void UnBindPiece();
	void StartGame();
	void GameOver(bool bIsWin);
	void Cinema();
	void Loading();
private:
	UPROPERTY()
	UUserWidget* CurrentWidget;
};
