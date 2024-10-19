// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/JangKCPiece.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void AJangKCPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void AJangKCPiece::Attacking()
{
	FDamageEvent DamageEvent;
	UGameplayStatics::ApplyDamage(AttackTarget.Get(), Damage, GetController(), this, UDamageType::StaticClass());
}

void AJangKCPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}

void AJangKCPiece::ActiveStart()
{
	SetState(EPieceState::ACTIVESKILL);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(SkillMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "ActiveEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, SkillMontage);
}

void AJangKCPiece::ActiveDoing()
{
	if (AttackTarget != nullptr) {
		float TargetHP = AttackTarget->GetHP();
		float FinalDamage = 9999.0f;
		FDamageEvent DamageEvent;
		UGameplayStatics::ApplyDamage(AttackTarget.Get(), FinalDamage, GetController(), this, UDamageType::StaticClass());
		PlaySkillEffect(AttackTarget->GetActorLocation());
	}
}

void AJangKCPiece::ActiveEnd()
{
	Super::ActiveEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
	StopSkillEffect();
}

void AJangKCPiece::PlaySkillEffect_Implementation(FVector EffectVector)
{
	StormComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffect, EffectVector, FRotator());
}

void AJangKCPiece::StopSkillEffect_Implementation()
{
	StormComponent->Deactivate();
}
