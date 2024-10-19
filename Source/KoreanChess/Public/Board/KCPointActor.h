// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Data/KCData.h"
#include "GameFramework/Actor.h"
#include "KCPointActor.generated.h"

UCLASS()
class KOREANCHESS_API AKCPointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AKCPointActor();
	AKCPointActor(const FObjectInitializer& ObjectInitializer);
	void SetPointInfo(FBoardPointInfo NewPointInfo);
	void SetState(EPointState NewState);
	FVector GetPointLocation();
	TWeakObjectPtr<class AKCPiece> GetOnPiece() { return OnPiece; }

	FPos GetPosition() { return Position; }
	EPointState GetState() { return State; }
protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void ClickedBox();
	UFUNCTION()
	void BeginOverCapsule();
	UFUNCTION()
	void OutOverCapsule();

	UFUNCTION(BlueprintCallable)
	void SetOnPiece(class AKCPiece* Actor);
	UFUNCTION(BlueprintCallable)
	void OutOnPiece(class AKCPiece* Actor);

	void UpdateParticle();
protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* CanMoveParticle;
	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(Replicated)
	FPos Position;

	EPointState State;

	UPROPERTY()
	TWeakObjectPtr<class AKCPiece> OnPiece;
};
