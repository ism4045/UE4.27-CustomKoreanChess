// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KCPiece.h"
#include "ChaPiece.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AChaPiece : public AKCPiece
{
	GENERATED_BODY()
	virtual void AttackStart() override;
	virtual void Attacking() override;
	UFUNCTION()
	void AttackEnd();
private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AChaProjectile> Projectile;
};
