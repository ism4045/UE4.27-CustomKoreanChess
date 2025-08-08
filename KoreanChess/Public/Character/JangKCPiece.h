// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/KCPiece.h"
#include "JangKCPiece.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AJangKCPiece : public AKCPiece
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
	UFUNCTION(NetMulticast, Reliable)
	void StopSkillEffect();
	void StopSkillEffect_Implementation();
private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UParticleSystem* SkillEffect;
	UPROPERTY()
	class UParticleSystemComponent* StormComponent;
};
