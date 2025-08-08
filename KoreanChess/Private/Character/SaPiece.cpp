// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SaPiece.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

void ASaPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void ASaPiece::Attacking()
{
	FDamageEvent DamageEvent;
	UGameplayStatics::ApplyDamage(AttackTarget.Get(), Damage, GetController(), this, UDamageType::StaticClass());
}

void ASaPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}

void ASaPiece::ActiveStart()
{
	SetState(EPieceState::ACTIVESKILL);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(SkillMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "ActiveEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, SkillMontage);
}

void ASaPiece::ActiveDoing()
{
	if (AttackTarget != nullptr) {
		float TargetHP = AttackTarget->GetHP();
		float FinalDamage = FMath::RoundToFloat(TargetHP * 0.3f);
		FDamageEvent DamageEvent;
		UGameplayStatics::ApplyDamage(AttackTarget.Get(), FinalDamage, GetController(), this, UDamageType::StaticClass());
		PlaySkillEffect(AttackTarget->GetActorLocation());
	}
}

void ASaPiece::ActiveEnd()
{
	Super::ActiveEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}

void ASaPiece::PlaySkillEffect_Implementation(FVector EffectVector)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffect, EffectVector, FRotator());
}
