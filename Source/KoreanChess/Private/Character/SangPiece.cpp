// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SangPiece.h"
#include "Projectile/ChaProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASangPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void ASangPiece::Attacking()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = this;
	FVector StartPos = GetMesh()->GetSocketLocation(FName("Muzzle_01"));
	FRotator Rotation = (AttackTarget->GetActorLocation() - StartPos).Rotation();
	AChaProjectile* Bullet = GetWorld()->SpawnActor<AChaProjectile>(Projectile, StartPos, Rotation, SpawnParameters);
	Bullet->SetAttackTarget(AttackTarget);
}

void ASangPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}

void ASangPiece::ReCoveryHP(float Amount)
{
	CurrentHP += Amount;
	if (CurrentHP > 100)
		CurrentHP = 100;
}
