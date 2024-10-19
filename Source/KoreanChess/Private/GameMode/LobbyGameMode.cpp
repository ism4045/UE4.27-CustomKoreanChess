// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameInstance/KCGameInstance.h"
#include "PlayerController/LobbyPlayerController.h"

void ALobbyGameMode::AddPlayerName(FString PlayerName)
{
	PlayerNames.Add(PlayerName);
	if (PlayerNames.Num() < 2) {
		PlayerOrders.Add(true);
	}
	else {
		PlayerOrders.Add(!PlayerOrders[0]);
	}
	FillGameInstances();
}
void ALobbyGameMode::ChangePlayerOrders()
{
	for (int32 i = 0; i < PlayerOrders.Num(); i++) {
		PlayerOrders[i] = !PlayerOrders[i];
	}
	for (int32 i = 0; i < PlayerControllers.Num(); i++) {
		PlayerControllers[i]->FillPlayerNames(PlayerNames, PlayerOrders[i]);
	}
}
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyPlayerController* PlayerController = Cast<ALobbyPlayerController>(NewPlayer);
	if (PlayerController == nullptr)
		return;
	PlayerControllers.Add(PlayerController);
	PlayerController->GetMyNameToInstance();
}

void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (GetNumPlayers() >= 2)
	{
		ErrorMessage = TEXT("Server full.");
		FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
		return;
	}

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	int32 IndexNum = 0;
	for (int32 i = 0; i < PlayerControllers.Num(); i++) {
		if (PlayerControllers[i] == Exiting) {
			PlayerControllers.RemoveAt(i);
			IndexNum = i;
			break;
		}
	}
	if (!bIsServerTravel) {
		FString RemovePlayerName = PlayerNames[IndexNum];
		PlayerNames.RemoveAt(IndexNum);
		for (int32 i = 0; i < PlayerControllers.Num(); i++) {
			PlayerControllers[i]->RemovePlayerInfo(RemovePlayerName);
		}
	}
}

void ALobbyGameMode::FillGameInstances()
{
	for (int32 i = 0; i < PlayerControllers.Num(); i++) {
		PlayerControllers[i]->FillPlayerNames(PlayerNames, PlayerOrders[i]);
	}
}
