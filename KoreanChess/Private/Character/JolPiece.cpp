// Fill out your copyright notice in the Description page of Project Settings.


#include "JolPiece.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

void AJolPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void AJolPiece::Attacking()
{
	FDamageEvent DamageEvent;
	int32 Result = FMath::RandRange(1, 100);
	int32 FinalDamage = Result <= CriticalPercentage ? Damage * 2 : Damage;
	UGameplayStatics::ApplyDamage(AttackTarget.Get(), FinalDamage, GetController(), this, UDamageType::StaticClass());
}

void AJolPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}