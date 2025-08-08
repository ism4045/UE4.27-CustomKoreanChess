// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
public:
	UFUNCTION(Client, Reliable)
	void GetMyNameToInstance();
	void GetMyNameToInstance_Implementation();

	UFUNCTION(Client, Reliable)
	void SetIsFirstOrderForInstance(bool NewbIsFirstOrder);
	void SetIsFirstOrderForInstance_Implementation(bool NewbIsFirstOrder);

	UFUNCTION(Client, Reliable)
	void RemovePlayerInfo(const FString& RemovePlayerName);
	void RemovePlayerInfo_Implementation(const FString& RemovePlayerName);

	UFUNCTION(Client, Reliable)
	void FillPlayerNames(const TArray<FString>& PlayerNames, bool NewbIsFirstOrder);
	void FillPlayerNames_Implementation(const TArray<FString>& PlayerNames, bool NewbIsFirstOrder);

	UFUNCTION(Server, Reliable)
	void SetMyName(const FString& NewMyName);
	void SetMyName_Implementation(const FString& NewMyName);

	UFUNCTION(Server, Reliable)
	void SetIsFirstOrder(bool NewbIsFirstOrder);
	void SetIsFirstOrder_Implementation(bool NewbIsFirstOrder);

	UFUNCTION(Server, Reliable)
	void ChangeOrder();
	void ChangeOrder_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ExitServer();
	void ExitServer_Implementation();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void ChangeToLoadingScreen();
	void ChangeToLoadingScreen_Implementation();

private:
	UPROPERTY(Replicated)
	FString MyName;
	UPROPERTY(Replicated)
	bool bIsFirstOrder;
};
