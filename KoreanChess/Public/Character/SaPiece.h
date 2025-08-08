// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Character/KCPiece.h"
#include "SaPiece.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API ASaPiece : public AKCPiece
{
	GENERATED_BODY()
public:
	virtual void AttackStart() override;
	virtual void Attacking() override;
	UFUNCTION()
	virtual void AttackEnd() override;

	virtual void ActiveStart() override;
	virtual void ActiveDoing() override;
	UFUNCTION()
	virtual void ActiveEnd() override;
	UFUNCTION(NetMulticast, Reliable)
	void PlaySkillEffect(FVector EffectVector);
	void PlaySkillEffect_Implementation(FVector EffectVector);
private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UParticleSystem* SkillEffect;
};
