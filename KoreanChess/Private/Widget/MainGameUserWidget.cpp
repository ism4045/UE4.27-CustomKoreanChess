// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainGameUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/KCPiece.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "GameState/KCGameStateBase.h"
#include "GameInstance/KCGameInstance.h"
#include "Components/Button.h"
#include "PlayerController/KoreanChessPlayerController.h"

void UMainGameUserWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	SetOpponentNameText();
	SetOpponentFirstOrderText();
	SetMyNameText();
	SetMyFirstOrderText();

	TurnEndButton->bIsEnabledDelegate.BindUFunction(this, "SetTurnEndButtonEnabled");
	HPBar->PercentDelegate.BindUFunction(this, "SetHPBar");
	MaxHPText->TextDelegate.BindUFunction(this, "SetMaxHPText");
	CurrentHPText->TextDelegate.BindUFunction(this, "SetCurrentHPText");
	DamageText->TextDelegate.BindUFunction(this, "SetDamageText");
	SkillTypeText->TextDelegate.BindUFunction(this, "SetSkillTypeText");
	ResurrectionText->TextDelegate.BindUFunction(this, "SetResurrectionText");

	LimitTimeText->TextDelegate.BindUFunction(this, "SetLimitTimeText");
	CurrentFirstOrderText->TextDelegate.BindUFunction(this, "SetCurrentFirstOrderText");

	OpponentNameText->TextDelegate.BindUFunction(this, "SetOpponentNameText");
	OpponentFirstOrderText->TextDelegate.BindUFunction(this, "SetOpponentFirstOrderText");
	OpponentScoreText->TextDelegate.BindUFunction(this, "SetOpponentScoreText");

	MyNameText->TextDelegate.BindUFunction(this, "SetMyNameText");
	MyFirstOrderText->TextDelegate.BindUFunction(this, "SetMyFirstOrderText");
	MyScoreText->TextDelegate.BindUFunction(this, "SetMyScoreText");

	HPBar->PercentDelegate.Execute();
	MaxHPText->TextDelegate.Execute();
	CurrentHPText->TextDelegate.Execute();
	DamageText->TextDelegate.Execute();
	SkillTypeText->TextDelegate.Execute();
	ResurrectionText->TextDelegate.Execute();

	LimitTimeText->TextDelegate.Execute();
	CurrentFirstOrderText->TextDelegate.Execute();

	OpponentScoreText->TextDelegate.Execute();
	MyScoreText->TextDelegate.Execute();

	TurnEndButton->OnClicked.AddDynamic(this, &UMainGameUserWidget::ChangeNextTurn);
}

void UMainGameUserWidget::BindSelectedCharacter(TWeakObjectPtr<class AKCPiece> NewSelectedCharacter)
{
	SelectedCharacter = NewSelectedCharacter;
	CharacterStatus->SetVisibility(ESlateVisibility::HitTestInvisible);
	ChangePieceStatusUI();
	PlayAnimation(Appear);
}

void UMainGameUserWidget::UnBindSelectedCharacter()
{
	SelectedCharacter.Reset();
	CharacterStatus->SetVisibility(ESlateVisibility::Hidden);
}

FText UMainGameUserWidget::SetMaxHPText()
{
	if (SelectedCharacter == nullptr)
		return FText();

	return FText::FromString(FString::FromInt(SelectedCharacter->GetMaxHP()));
}

FText UMainGameUserWidget::SetCurrentHPText()
{
	if (SelectedCharacter == nullptr)
		return FText();

	return FText::FromString(FString::FromInt(SelectedCharacter->GetHP()));
}

FText UMainGameUserWidget::SetDamageText()
{
	if (SelectedCharacter == nullptr)
		return FText();

	return FText::FromString(FString::FromInt(SelectedCharacter->GetDamage()));
}

FText UMainGameUserWidget::SetSkillTypeText()
{
	if (SelectedCharacter == nullptr)
		return FText();

	if (SelectedCharacter->GetSkillType() == ESkillType::PASSIVE) {
		return PassiveKorean;
	}
	else {
		return ActiveKorean;
	}
}

float UMainGameUserWidget::SetHPBar()
{
	if (SelectedCharacter == nullptr)
		return 0.0f;

	return SelectedCharacter->GetHpPercent();
}

FText UMainGameUserWidget::SetResurrectionText()
{
	if (SelectedCharacter == nullptr)
		return FText();
	if (SelectedCharacter->GetKind() == EPieceKinds::JOL || SelectedCharacter->GetKind() == EPieceKinds::MA || SelectedCharacter->GetKind() == EPieceKinds::SANG) {
		int32 NeedScore = SelectedCharacter->GetDeathScore() * 3;
		FString Result = ResurrectionTextArray[0].ToString() + FString::FromInt(NeedScore) + ")";
		return FText::FromString(Result);
	}
	return ResurrectionTextArray[1];
}

void UMainGameUserWidget::ChangePieceStatusUI()
{
	EPieceKinds Kind = SelectedCharacter->GetKind();
	EPieceTeam Team = SelectedCharacter->GetTeam();

	if (Team == EPieceTeam::BLACK) {
		Character_RenderTarget->SetBrushResourceObject(BlackMatArray[(int32)Kind]);
	}
	else {
		Character_RenderTarget->SetBrushResourceObject(WhilteMatArray[(int32)Kind]);
	}
	PieceNameText->SetText(PieceNameArray[(int32)Kind]);
	DescriptionText->SetText(SkillDescriptionArray[(int32)Kind]);
	DirectionText->SetText(DirectionArray[(int32)Kind]);
}

FText UMainGameUserWidget::SetLimitTimeText() {
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FText();
	return FText::FromString(FString::FromInt(GameState->GetLimitTime()));
}

FText UMainGameUserWidget::SetCurrentFirstOrderText() {
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameState == nullptr)
		return FText();
	EPieceTeam FirstOrder = GameState->GetCurrentTurn();
	if (FirstOrder == EPieceTeam::BLACK) {
		return FirstOrderKorean;
	}
	else {
		return SecondOrderKorean;
	}
}

void UMainGameUserWidget::SetOpponentNameText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		OpponentNameText->SetText(FText());
	TArray<FString> NameArray = GameInstance->GetPlayerNames();
	FString MyName = GameInstance->GetMyName();
	for (int32 i = 0; i < NameArray.Num(); i++) {
		if (MyName != NameArray[i]) {
			OpponentNameText->SetText(FText::FromString(NameArray[i]));
			break;
		}
	}
}

void UMainGameUserWidget::SetOpponentFirstOrderText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		OpponentFirstOrderText->SetText(FText());
	if (GameInstance->GetIsFirstOrder() == true)
		OpponentFirstOrderText->SetText(SecondOrderKorean);
	else
		OpponentFirstOrderText->SetText(FirstOrderKorean);
}

FText UMainGameUserWidget::SetOpponentScoreText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameInstance == nullptr || GameState == nullptr)
		return FText();
	if (GameInstance->GetIsFirstOrder() == true)
		return FText::FromString(FString::FromInt(GameState->GetSecondOrderScore()));
	else
		return FText::FromString(FString::FromInt(GameState->GetFirstOrderScore()));
}

void UMainGameUserWidget::SetMyNameText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		MyNameText->SetText(FText());
	MyNameText->SetText(FText::FromString(GameInstance->GetMyName()));
}

void UMainGameUserWidget::SetMyFirstOrderText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance == nullptr)
		MyFirstOrderText->SetText(FText());
	if (GameInstance->GetIsFirstOrder() == true)
		MyFirstOrderText->SetText(FirstOrderKorean);
	else
		MyFirstOrderText->SetText(SecondOrderKorean);
}

FText UMainGameUserWidget::SetMyScoreText()
{
	UKCGameInstance* GameInstance = Cast<UKCGameInstance>(GetWorld()->GetGameInstance());
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	if (GameInstance == nullptr || GameState == nullptr)
		return FText();
	if (GameInstance->GetIsFirstOrder() == true)
		return FText::FromString(FString::FromInt(GameState->GetFirstOrderScore()));
	else
		return FText::FromString(FString::FromInt(GameState->GetSecondOrderScore()));
}

bool UMainGameUserWidget::SetTurnEndButtonEnabled()
{
	AKCGameStateBase* GameState = Cast<AKCGameStateBase>(GetWorld()->GetGameState());
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (GameState == nullptr || PlayerController == nullptr)
		return false;
	if (PlayerController->GetIsFirstOrder() == GameState->GetCurrentTurn())
		return true;
	return false;
}

void UMainGameUserWidget::ChangeNextTurn()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
		return ;
	PlayerController->TrySendMessage(EMessageType::TURNEND,nullptr);
}
