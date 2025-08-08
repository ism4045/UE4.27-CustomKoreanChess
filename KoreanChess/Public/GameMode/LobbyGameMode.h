// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	void AddPlayerName(FString PlayerName);
	void ChangePlayerOrders();
	UFUNCTION(BlueprintCallable)
	TArray<class ALobbyPlayerController*> GetAllPlayerControllers() { return PlayerControllers; }
	UFUNCTION(BlueprintCallable)
	void SetIsServerTravel(bool NewbIsServerTravel) { bIsServerTravel = NewbIsServerTravel; }
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void Logout(AController* Exiting) override;
	void FillGameInstances();
private:
	UPROPERTY()
	TArray<FString> PlayerNames;
	UPROPERTY()
	TArray<bool> PlayerOrders;
	UPROPERTY()
	TArray<class ALobbyPlayerController*> PlayerControllers;

	bool bIsServerTravel = false;
};
