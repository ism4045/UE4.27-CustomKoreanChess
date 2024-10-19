// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CustomCameraActor.h"
#include "Character/KCPiece.h"

ACustomCameraActor::ACustomCameraActor()
{
}

ACustomCameraActor::ACustomCameraActor(const FObjectInitializer& ObjectInitializer)
{
}


void ACustomCameraActor::SetCameraLocationAndRotation_Implementation(AKCPiece* Target)
{
	if (GetWorld()->IsServer()) {
		UE_LOG(LogTemp, Warning, TEXT("Server"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Client"));
	}
	if (Target == nullptr)
		return;

	if (Target->GetTeam() == EPieceTeam::BLACK) {
		SetActorRotation(FRotator(-30.0f, 0.0f, 0.0f));
	}
	else {
		SetActorRotation(FRotator(-30.0f, 180.0f, 0.0f));
	}
	SetActorLocation(FVector(440.0f, Target->GetActorLocation().Y, 400.0f));
}
