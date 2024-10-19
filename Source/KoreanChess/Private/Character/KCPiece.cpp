// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/KCPiece.h"
#include "Components/CapsuleComponent.h"
#include "PlayerController/KoreanChessPlayerController.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/PieceController.h"
#include "Sound/SoundCue.h"
#include "GameMode/KoreanChessGameMode.h"
#include "Components/WidgetComponent.h"
#include "Widget/HPBarUserWidget.h"
#include "Widget/SelectPieceUserWidget.h"
#include "HUD/KCHUD.h"

// Sets default values
AKCPiece::AKCPiece()
{
}

AKCPiece::AKCPiece(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	CurrentHP = 100;
	State = EPieceState::IDLE;
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = NULL;
	bUseControllerRotationYaw = false;

	CanAttackParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("CanAttackParticle"));
	CanAttackParticle->SetupAttachment(GetMesh());

	OutLineMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("OutLineMesh"));
	OutLineMesh->SetupAttachment(GetMesh());

	HPWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("HPWidget"));
	HPWidget->SetupAttachment(RootComponent);
	HPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidget->SetDrawSize(FVector2D(75.0f, 10.0f));
	HPWidget->SetVisibility(true);

	SelectWidget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("SelectWidget"));
	SelectWidget->SetupAttachment(RootComponent);
	SelectWidget->SetWidgetSpace(EWidgetSpace::Screen);
	SelectWidget->SetDrawSize(FVector2D(10.0f, 10.0f));
	SelectWidget->SetRelativeLocation(FVector(-60.0f, 50.0f, 0.0f));
	SelectWidget->SetRelativeRotation(FRotator(90.0f, 0.0f, 180.0f));
	SelectWidget->SetVisibility(false);

	GetCapsuleComponent()->SetCapsuleHalfHeight(60.0f);
	GetCapsuleComponent()->SetCollisionProfileName("OnlyBlockDynamic");

	GetMesh()->SetCollisionProfileName("NoCollision");

	CanAttackParticle->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	CanAttackParticle->SetVisibility(false);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	BlueMaterial = CreateDefaultSubobject<UMaterialInterface>("BlueMaterial");
	RedMaterial = CreateDefaultSubobject<UMaterialInterface>("RedMaterial");
	GreenMaterial = CreateDefaultSubobject<UMaterialInterface>("GreenMaterial");
	BlackMaterial = CreateDefaultSubobject<UMaterialInterface>("BlackMaterial");
}

void AKCPiece::SetAttackTarget(TWeakObjectPtr<AKCPiece> NewAttackTarget)
{
	AttackTarget = NewAttackTarget;
}

void AKCPiece::SetState(EPieceState NewState)
{
	State = NewState;
	UpdateParticle();
}

void AKCPiece::SelectThis()
{
	ChangeColor(EColorType::GREEN);
	if (State != EPieceState::DEAD) {
		SelectWidget->SetVisibility(true);
	}
}

void AKCPiece::DeSelectThis()
{
	ChangeColor(EColorType::BLACK);
	SelectWidget->SetVisibility(false);
}

// Called when the game starts or when spawned
void AKCPiece::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "ClickedCapsule");
	GetCapsuleComponent()->OnClicked.Add(Delegate);
	Delegate.Clear();
	Delegate.BindUFunction(this, "BeginOverCapsule");
	GetCapsuleComponent()->OnBeginCursorOver.Add(Delegate);
	Delegate.Clear();
	Delegate.BindUFunction(this, "OutOverCapsule");
	GetCapsuleComponent()->OnEndCursorOver.Add(Delegate);

	HPWidget->SetWidgetClass(HPBarWB);
	SelectWidget->SetWidgetClass(ActionSelectWB);
	Cast<USelectPieceUserWidget>(SelectWidget->GetWidget())->SetOwnerPiece(this);
	CurrentHP = MaxHP;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), FRotator(), FVector(0.5f,0.5f,0.5f));
}

void AKCPiece::Destroyed()
{
	if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy && Instigator != nullptr && State != EPieceState::DEAD) {
		AKoreanChessGameMode* GameMode = Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode && Instigator.IsValid()) {
			GameMode->CheckCurrentTurn(EActionInTurn::DEAD, this);
		}
		if (Instigator.IsValid()) {
			Instigator->PieceMove(GetActorLocation());
		}
	}
	Super::Destroyed();
}

float AKCPiece::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHP -= DamageAmount;
	if (DamageAmount >= 9999.0f)
	if (Cast<AKCPiece>(DamageCauser)->GetKind() == EPieceKinds::SA && Cast<AKCPiece>(DamageCauser)->GetState() == EPieceState::ACTIVESKILL) {
		if (CurrentHP < 1.0f)
			CurrentHP = 1.0f;
	}
	Cast<UHPBarUserWidget>(HPWidget->GetWidget())->ReactDamage(GetHpPercent());
	if (CurrentHP > 0.0f) {
		PlayEffect(HitReactMontage);
	}
	else {
		PlayEffect(DeathMontage);
		if (Cast<AKCPiece>(DamageCauser)->GetKind() == EPieceKinds::JANG && Cast<AKCPiece>(DamageCauser)->GetState() == EPieceState::ACTIVESKILL) {
			Instigator = nullptr;
		}
		else {
			Instigator = Cast<APieceController>(EventInstigator);
		}
		if (Kind == EPieceKinds::JANG) {
			Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode())->PlaySequence(this);
			Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode())->DeclineAllControllerInput();
		}
	}
	return DamageAmount;
}

void AKCPiece::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKCPiece, CurrentHP);
	DOREPLIFETIME(AKCPiece, CoolTime);
	DOREPLIFETIME(AKCPiece, State);
}

void AKCPiece::ClickedCapsule()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		if ((State == EPieceState::IDLE || State == EPieceState::DEAD) && Team == PlayerController->GetIsFirstOrder()) {
			PlayerController->SelectPiece(this);
			PlayerController->SetCurrentCursor(EMouseCursor::Default);
		}
		else if (State == EPieceState::CANDAMAGED) {
			PlayerController->TrySendMessage(EMessageType::ATTACK,this, EActionMessageType::MOVEORATTACK);
			PlayerController->SetCurrentCursor(EMouseCursor::Default);
		}
		else if (State == EPieceState::CANSKILLDAMAGED) {
			PlayerController->TrySendMessage(EMessageType::ATTACK, this, EActionMessageType::ACTIVESKILL);
			PlayerController->SetCurrentCursor(EMouseCursor::Default);
		}
	}
}

void AKCPiece::BeginOverCapsule()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	AKCHUD* HUD = Cast<AKCHUD>(PlayerController->GetHUD());
	HUD->BindPiece(this);
	if (PlayerController && CurrentColorType == EColorType::BLACK) {
		if (Team == PlayerController->GetIsFirstOrder()) {
			ChangeColor(EColorType::BLUE);
			PlayerController->SetCurrentCursor(EMouseCursor::Hand);
		}
		else {
			TArray<class UMaterialInterface*> MaterialArr = OutLineMesh->GetMaterials();
			ChangeColor(EColorType::RED);
			if (State == EPieceState::CANDAMAGED) {
				PlayerController->SetCurrentCursor(EMouseCursor::GrabHand);
			}
		}
	}
}

void AKCPiece::OutOverCapsule()
{
	if (CurrentColorType != EColorType::GREEN) {
		ChangeColor(EColorType::BLACK);
	}
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		PlayerController->SetCurrentCursor(EMouseCursor::Default);
		AKCHUD* HUD = Cast<AKCHUD>(PlayerController->GetHUD());
		HUD->UnBindPiece();
	}
}

void AKCPiece::ChangeColor(EColorType NewColor)
{
	UMaterialInterface* FinalMaterial = nullptr;
	switch (NewColor)
	{
	case EColorType::RED:
		FinalMaterial = RedMaterial;
		break;
	case EColorType::GREEN:
		FinalMaterial = GreenMaterial;
		break;
	case EColorType::BLUE:
		FinalMaterial = BlueMaterial;
		break;
	case EColorType::BLACK:
		FinalMaterial = BlackMaterial;
		break;
	default:
		break;
	}

	TArray<class UMaterialInterface*> MaterialArr = OutLineMesh->GetMaterials();
	for (int32 i = 0; i < MaterialArr.Num(); i++) {
		if (MaterialArr[i] == BlueMaterial || MaterialArr[i] == RedMaterial || MaterialArr[i] == GreenMaterial || MaterialArr[i] == BlackMaterial) {
			OutLineMesh->SetMaterial(i, FinalMaterial);
		}
	}
	CurrentColorType = NewColor;
}

void AKCPiece::PlayEffect_Implementation(UAnimMontage* AnimMontage)
{
	GetMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
	OutLineMesh->GetAnimInstance()->Montage_Play(AnimMontage);
}

void AKCPiece::UpdateParticle()
{
	if (State == EPieceState::CANDAMAGED || State == EPieceState::CANSKILLDAMAGED) {
		CanAttackParticle->SetVisibility(true);
	}
	else {
		CanAttackParticle->SetVisibility(false);
	}
}

void AKCPiece::InitializeRotation()
{
	if (Team == EPieceTeam::WHITE) {
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else {
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	}
}

float AKCPiece::GetHpPercent()
{
	return CurrentHP / MaxHP;
}

void AKCPiece::OnRep_ChangeHP()
{
	if (Cast<UHPBarUserWidget>(HPWidget->GetWidget()) != nullptr) {
		Cast<UHPBarUserWidget>(HPWidget->GetWidget())->ReactDamage(GetHpPercent());
	}
}

void AKCPiece::ActiveEnd()
{
	if (GetWorld()->IsServer()) {
		AKoreanChessGameMode* GameMode = Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode) {
			GameMode->CheckCurrentTurn(EActionInTurn::ACTIVESKILL);
		}
	}
}

void AKCPiece::AttackEnd()
{
	if (GetWorld()->IsServer()) {
		AKoreanChessGameMode* GameMode = Cast<AKoreanChessGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode) {
			GameMode->CheckCurrentTurn(EActionInTurn::ATTACK);
		}
	}
}

void AKCPiece::OnDeath()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEffect, GetActorLocation(), FRotator(), FVector(0.5f, 0.5f, 0.5f));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeathEffectSound, GetActorLocation());
	Destroy();
}

// Called to bind functionality to input
void AKCPiece::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKCPiece::SelectButtonClicked()
{
	SelectWidget->SetVisibility(false);
}

