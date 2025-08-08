// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API UKCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	UFUNCTION()
	void AnimNotify_Attack();
	UFUNCTION()
	void AnimNotify_Death();
	UFUNCTION()
	void AnimNotify_ActiveSkill();
protected:
	UPROPERTY(BlueprintReadWrite, Category = Movement)
	float Speed;

	TWeakObjectPtr<class AKCPiece> Owner;
};
