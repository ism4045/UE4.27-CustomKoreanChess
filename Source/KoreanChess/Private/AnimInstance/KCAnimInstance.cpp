// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/KCAnimInstance.h"
#include "Character/KCPiece.h"
#include "Kismet/GameplayStatics.h"

void UKCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Speed = 0.0f;
	Owner = Cast<AKCPiece>(TryGetPawnOwner());
}

void UKCAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (Owner == nullptr)
		return;

	Speed = Owner->GetVelocity().Size();
}

void UKCAnimInstance::AnimNotify_Attack()
{
	if (Owner->GetMesh() == GetOwningComponent()) {
		if (GetWorld()->IsServer()) {
			Owner->Attacking();
		}
	}
}

void UKCAnimInstance::AnimNotify_Death()
{
	if (Owner->GetMesh() == GetOwningComponent()) {
		Owner->OnDeath();
	}
}

void UKCAnimInstance::AnimNotify_ActiveSkill()
{
	if (Owner->GetMesh() == GetOwningComponent()) {
		if (GetWorld()->IsServer()) {
			Owner->ActiveDoing();
		}
	}
}
