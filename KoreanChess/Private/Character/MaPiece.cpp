// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MaPiece.h"
#include "Projectile/ChaProjectile.h"
#include "Kismet/GameplayStatics.h"

void AMaPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void AMaPiece::Attacking()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = this;
	FVector StartPos = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FRotator Rotation = (AttackTarget->GetActorLocation() - StartPos).Rotation();
	AChaProjectile* Bullet = GetWorld()->SpawnActor<AChaProjectile>(Projectile, StartPos, Rotation, SpawnParameters);
	Bullet->SetAttackTarget(AttackTarget);
}

void AMaPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}

void AMaPiece::ReCoveryHP(float Amount)
{
	CurrentHP += Amount;
	if (CurrentHP > 100)
		CurrentHP = 100;
}
