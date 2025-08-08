// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaProjectile.generated.h"

UCLASS()
class KOREANCHESS_API AChaProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChaProjectile();
	void SetAttackTarget(TWeakObjectPtr<class AKCPiece> NewAttackTarget) { AttackTarget = NewAttackTarget; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION(Category = "Projectile")
	void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent ,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* ProjectileEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundCue* ExplosionSound;

	TWeakObjectPtr<class AKCPiece> AttackTarget;
};
