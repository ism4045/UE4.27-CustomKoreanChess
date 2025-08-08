// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Character/KCPiece.h"
#include "JolPiece.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AJolPiece : public AKCPiece
{
	GENERATED_BODY()
public:
	virtual void AttackStart() override;
	virtual void Attacking() override;
	UFUNCTION()
	virtual void AttackEnd() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Status)
	int32 CriticalPercentage;
};
