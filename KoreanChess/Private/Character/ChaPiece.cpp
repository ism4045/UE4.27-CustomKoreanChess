// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaPiece.h"
#include "ChaProjectile.h"

#include "Kismet/GameplayStatics.h"

void AChaPiece::AttackStart()
{
	SetState(EPieceState::ATTACK);
	SetActorRotation(FVector(AttackTarget->GetActorLocation() - GetActorLocation()).Rotation());
	PlayEffect(AttackMontage);
	FOnMontageEnded MontageEnded;
	MontageEnded.BindUFunction(this, "AttackEnd");
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageEnded, AttackMontage);
}

void AChaPiece::Attacking()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = this;
	FVector StartPos = GetMesh()->GetSocketLocation(FName("Muzzle_Front"));
	FRotator Rotation = (AttackTarget->GetActorLocation() - StartPos).Rotation();
	AChaProjectile* Bullet = GetWorld()->SpawnActor<AChaProjectile>(Projectile, StartPos, Rotation, SpawnParameters);
	Bullet->SetAttackTarget(AttackTarget);
}

void AChaPiece::AttackEnd()
{
	Super::AttackEnd();
	SetState(EPieceState::IDLE);
	InitializeRotation();
	AttackTarget.Reset();
}
