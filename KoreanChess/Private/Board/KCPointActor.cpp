// Fill out your copyright notice in the Description page of Project Settings.


#include "KCPointActor.h"
#include "KoreanChessPlayerController.h"
#include "KCPiece.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

AKCPointActor::AKCPointActor()
{
}

// Sets default values
AKCPointActor::AKCPointActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	CanMoveParticle = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("CanMoveParticle"));
	CanMoveParticle->SetupAttachment(RootComponent);
}

void AKCPointActor::SetPointInfo(FBoardPointInfo NewPointInfo)
{
	Position = NewPointInfo.Position;
	State = NewPointInfo.State;
}

void AKCPointActor::SetState(EPointState NewState)
{
	State = NewState;
	UpdateParticle();
}

FVector AKCPointActor::GetPointLocation()
{
	return BoxCollision->GetComponentLocation();
}

// Called when the game starts or when spawned
void AKCPointActor::BeginPlay()
{
	Super::BeginPlay();
	if (BoxCollision) {
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, "ClickedBox");
		BoxCollision->OnClicked.Add(Delegate);
		Delegate.Clear();
		Delegate.BindUFunction(this, "BeginOverCapsule");
		BoxCollision->OnBeginCursorOver.Add(Delegate);
		Delegate.Clear();
		Delegate.BindUFunction(this, "OutOverCapsule");
		BoxCollision->OnEndCursorOver.Add(Delegate);
	}
}

void AKCPointActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AKCPointActor, Position);
}

void AKCPointActor::ClickedBox()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController && (State == EPointState::CANMOVE || State == EPointState::CANSPAWN)) {
		EMessageType SendType = State == EPointState::CANMOVE ? EMessageType::MOVE : EMessageType::SPAWN;
		PlayerController->TrySendMessage(SendType, this, EActionMessageType::MOVEORATTACK);
		PlayerController->SetCurrentCursor(EMouseCursor::Default);
	}
}

void AKCPointActor::BeginOverCapsule()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController && State == EPointState::CANMOVE) {
		PlayerController->SetCurrentCursor(EMouseCursor::GrabHandClosed);
	}
}

void AKCPointActor::OutOverCapsule()
{
	AKoreanChessPlayerController* PlayerController = Cast<AKoreanChessPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController) {
		PlayerController->SetCurrentCursor(EMouseCursor::Default);
	}
}

void AKCPointActor::SetOnPiece(AKCPiece* Actor)
{
	if (OnPiece != nullptr)
		return;
	Actor->SetCurrentPos(Position);
	OnPiece = Actor;
}

void AKCPointActor::OutOnPiece(AKCPiece* Actor)
{
	if (OnPiece == Actor)
		OnPiece = nullptr;
}

void AKCPointActor::UpdateParticle()
{
	switch (State)
	{
	case EPointState::NONE:
		CanMoveParticle->SetVisibility(false);
		break;
	case EPointState::CANSPAWN:
	case EPointState::CANMOVE:
		CanMoveParticle->SetVisibility(true);
		break;
	default:
		break;
	}
}