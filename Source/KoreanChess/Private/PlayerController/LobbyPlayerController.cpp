// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/LobbyPlayerController.h"
#include "GameInstance/KCGameInstance.h"
#include "GameMode/LobbyGameMode.h"
#include "HUD/KCHUD.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!HasAuthority() || GetRemoteRole() == ENetRole::ROLE_SimulatedProxy) {
		SetShowMouseCursor(true);
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPlayerController, MyName);
}
void ALobbyPlayerController::GetMyNameToInstance_Implementation()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	FString InstanceMyName = GameInstance->GetMyName();
	SetMyName(InstanceMyName);
}

void ALobbyPlayerController::SetIsFirstOrderForInstance_Implementation(bool NewbIsFirstOrder)
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	GameInstance->SetIsFirstOrder(NewbIsFirstOrder);
	SetIsFirstOrder(NewbIsFirstOrder);
}

void ALobbyPlayerController::RemovePlayerInfo_Implementation(const FString& RemovePlayerName)
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	TArray<FString> PlayerNames = GameInstance->GetPlayerNames();
	PlayerNames.Remove(RemovePlayerName);
	GameInstance->SetPlayerNames(PlayerNames);

	AKCHUD* KCHUD = Cast<AKCHUD>(GetHUD());
	if (KCHUD == nullptr)
		return;
	KCHUD->UpdateLobbyMember();
}

void ALobbyPlayerController::FillPlayerNames_Implementation(const TArray<FString>& PlayerNames, bool NewbIsFirstOrder)
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	GameInstance->SetPlayerNames(PlayerNames);
	GameInstance->SetIsFirstOrder(NewbIsFirstOrder);
	AKCHUD* KCHUD = Cast<AKCHUD>(GetHUD());
	if (KCHUD == nullptr)
		return;
	KCHUD->UpdateLobbyMember();
}

void ALobbyPlayerController::SetMyName_Implementation(const FString& NewMyName)
{
	MyName = NewMyName;
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
		return;
	GameMode->AddPlayerName(MyName);
}

void ALobbyPlayerController::SetIsFirstOrder_Implementation(bool NewbIsFirstOrder)
{
	bIsFirstOrder = NewbIsFirstOrder;
}

void ALobbyPlayerController::ChangeOrder_Implementation()
{
	ALobbyGameMode* GameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr)
		return;
	GameMode->ChangePlayerOrders();
}

void ALobbyPlayerController::ExitServer_Implementation()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		return;
	UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->GetMenuMapName()),true,"kicked");
}

void ALobbyPlayerController::ChangeToLoadingScreen_Implementation()
{
	AKCHUD* KCHUD = Cast<AKCHUD>(GetHUD());
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (KCHUD == nullptr || GameInstance == nullptr)
		return;
	KCHUD->ChangeWidget(GameInstance->GetLoadingWidget());
}
