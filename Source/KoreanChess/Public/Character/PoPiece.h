// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KCPiece.h"
#include "PoPiece.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API APoPiece : public AKCPiece
{
	GENERATED_BODY()
public:
	virtual void AttackStart() override;
	virtual void Attacking() override;
	UFUNCTION()
	virtual void AttackEnd() override;
	void IncreaseDamage();
private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AChaProjectile> Projectile;
};
