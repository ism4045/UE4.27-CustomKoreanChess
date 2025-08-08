// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Engine/GameInstance.h"
#include "KCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UKCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;
public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetMenuWidget() { return MenuWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLobbyWidget() { return LobbyWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetServerBrowserWidget() { return ServerBrowserWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetServerSettingWidget() { return ServerSettingWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadingWidget() { return LoadingWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetDescriptionWidget() { return DescriptionWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetMainGameWidget() { return MainGameWB; }
	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetGameOverWidget() { return GameOverWB; }

	UFUNCTION(BlueprintCallable)
	FString GetMenuMapName() { return MenuMapName; }
	UFUNCTION(BlueprintCallable)
	FString GetLobbyMapName() { return LobbyMapName; }
	UFUNCTION(BlueprintCallable)
	FString GetChessMapName() { return ChessMapName; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerNames(TArray<FString> NewPlayerNames) { PlayerNames = NewPlayerNames; }
	UFUNCTION(BlueprintCallable)
	void SetMyName(FString NewName) { MyName = NewName; }
	UFUNCTION(BlueprintCallable)
	void SetServerName(FString NewServerName) { ServerName = NewServerName; }
	UFUNCTION(BlueprintCallable)
	void SetIsFirstOrder(bool NewbIsFirstOrder) { bIsFIrstOrder = NewbIsFirstOrder; }

	FString GetMyName() { return MyName; }
	FString GetServerName() { return ServerName; }
	TArray<FString> GetPlayerNames() { return PlayerNames; }
	bool GetIsFirstOrder() { return bIsFIrstOrder; }
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LoadingWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MenuWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ServerBrowserWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> ServerSettingWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> LobbyWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> DescriptionWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MainGameWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> GameOverWB;

	UPROPERTY(EditDefaultsOnly, Category = "MapNames")
	FString MenuMapName;
	UPROPERTY(EditDefaultsOnly, Category = "MapNames")
	FString LobbyMapName;
	UPROPERTY(EditDefaultsOnly, Category = "MapNames")
	FString ChessMapName;

	UPROPERTY()
	TArray<FString> PlayerNames;
	UPROPERTY()
	FString MyName;
	UPROPERTY()
	FString ServerName;
	UPROPERTY()
	bool bIsFIrstOrder = true;
};
