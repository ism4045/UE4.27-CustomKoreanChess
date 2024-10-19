// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PieceController.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API APieceController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PieceMove(FVector NewDestination);
	void PieceMove_Implementation(FVector NewDestination);
	void AttackTarget(class AKCPiece* NewTarget);
	void DoActiveSkill(class AKCPiece* NewTarget);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
private:
};
