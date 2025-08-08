// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/PieceController.h"
#include "Character/KCPiece.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

void APieceController::PieceMove_Implementation(FVector NewDestination)
{
}

void APieceController::AttackTarget(AKCPiece* NewTarget)
{
	AKCPiece* KCPiece = Cast<AKCPiece>(GetPawn());
	if (KCPiece) {
		KCPiece->SetAttackTarget(NewTarget);
		KCPiece->AttackStart();
	}
}

void APieceController::DoActiveSkill(AKCPiece* NewTarget)
{
	AKCPiece* KCPiece = Cast<AKCPiece>(GetPawn());
	if (KCPiece) {
		KCPiece->SetAttackTarget(NewTarget);
		KCPiece->ActiveStart();
	}
}

void APieceController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void APieceController::OnUnPossess()
{
	Super::OnUnPossess();
}
