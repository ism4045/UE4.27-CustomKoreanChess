// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/KCHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance/KCGameInstance.h"
#include "Widget/LobbyUserWidget.h"
#include "Widget/MainGameUserWidget.h"
#include "Widget/GameOverUserWidget.h"

void AKCHUD::BeginPlay()
{
    Super::BeginPlay();
    UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance == nullptr) 
        return;

    if (UGameplayStatics::GetCurrentLevelName(this) == GameInstance->GetMenuMapName()) {
        ChangeWidget(GameInstance->GetMenuWidget());
    }
    if (UGameplayStatics::GetCurrentLevelName(this) == GameInstance->GetLobbyMapName()) {
        ChangeWidget(GameInstance->GetLobbyWidget());
    }
    if (UGameplayStatics::GetCurrentLevelName(this) == GameInstance->GetChessMapName()) {
        ChangeWidget(GameInstance->GetDescriptionWidget());
    }
}

void AKCHUD::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }
    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
            CurrentWidget->SetOwningPlayer(GetOwningPlayerController());
        }
    }
}

void AKCHUD::UpdateLobbyMember()
{
    ULobbyUserWidget* LobbyWidget = Cast<ULobbyUserWidget>(CurrentWidget);
    if (LobbyWidget == nullptr)
        return;
    LobbyWidget->UpdateLobbyMember();
  
}

void AKCHUD::BindPiece(TWeakObjectPtr<class AKCPiece> NewSelectedCharacter)
{
    UMainGameUserWidget* MainGameWIdget = Cast<UMainGameUserWidget>(CurrentWidget);
    if (MainGameWIdget == nullptr)
        return;
    MainGameWIdget->BindSelectedCharacter(NewSelectedCharacter);
}

void AKCHUD::UnBindPiece()
{
    UMainGameUserWidget* MainGameWIdget = Cast<UMainGameUserWidget>(CurrentWidget);
    if (MainGameWIdget == nullptr)
        return;
    MainGameWIdget->UnBindSelectedCharacter();
}

void AKCHUD::StartGame() {
    UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance == nullptr)
        return;
    ChangeWidget(GameInstance->GetMainGameWidget());
}

void AKCHUD::GameOver(bool bIsWin)
{
    UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance == nullptr)
        return;
    ChangeWidget(GameInstance->GetGameOverWidget());
    Cast<UGameOverUserWidget>(CurrentWidget)->SetResultText(bIsWin);
}

void AKCHUD::Cinema()
{
    ChangeWidget(nullptr);
}

void AKCHUD::Loading()
{
    UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
    if (GameInstance == nullptr)
        return;
    ChangeWidget(GameInstance->GetLoadingWidget());
}
