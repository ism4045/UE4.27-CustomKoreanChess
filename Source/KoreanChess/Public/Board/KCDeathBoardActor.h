// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KCDeathBoardActor.generated.h"

UCLASS()
class KOREANCHESS_API AKCDeathBoardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKCDeathBoardActor();
	AKCDeathBoardActor(const FObjectInitializer& ObjectInitializer);
	FVector GetSpawnLocation();
	TWeakObjectPtr<class AKCPiece> GetOnPiece() { return OnPiece; }
	bool IsEmpty();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetOnPiece(class AKCPiece* Actor);
	UFUNCTION(BlueprintCallable)
	void OutOnPiece(class AKCPiece* Actor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* CubeStaticMesh;
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* LineStaticMesh1;
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* LineStaticMesh2;
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* LineStaticMesh3;
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* LineStaticMesh4;
	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY()
	TWeakObjectPtr<class AKCPiece> OnPiece;
};
