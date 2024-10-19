// Fill out your copyright notice in the Description page of Project Settings.


#include "Board/KCDeathBoardActor.h"
#include "Components/BoxComponent.h"
#include "Character/KCPiece.h"

// Sets default values
AKCDeathBoardActor::AKCDeathBoardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AKCDeathBoardActor::AKCDeathBoardActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CubeStaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("CubeStaticMesh"));
	RootComponent = CubeStaticMesh;
	LineStaticMesh1 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LineStaticMesh1"));
	LineStaticMesh1->SetupAttachment(RootComponent);
	LineStaticMesh2 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LineStaticMesh2"));
	LineStaticMesh2->SetupAttachment(RootComponent);
	LineStaticMesh3 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LineStaticMesh3"));
	LineStaticMesh3->SetupAttachment(RootComponent);
	LineStaticMesh4 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("LineStaticMesh4"));
	LineStaticMesh4->SetupAttachment(RootComponent);


	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

FVector AKCDeathBoardActor::GetSpawnLocation()
{
	return BoxCollision->GetComponentLocation();
}

// Called when the game starts or when spawned
void AKCDeathBoardActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKCDeathBoardActor::SetOnPiece(AKCPiece* Actor)
{
	if (OnPiece != nullptr)
		return;
	OnPiece = Actor;
}

void AKCDeathBoardActor::OutOnPiece(AKCPiece* Actor)
{
	if (OnPiece == Actor)
		OnPiece = nullptr;
}

bool AKCDeathBoardActor::IsEmpty()
{
	return OnPiece == nullptr;
}

// Called every frame
void AKCDeathBoardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

