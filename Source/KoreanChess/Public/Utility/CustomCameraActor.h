// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "Camera/CameraActor.h"
#include "CustomCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API ACustomCameraActor : public ACameraActor
{
	GENERATED_BODY()
public:
	ACustomCameraActor();
	ACustomCameraActor(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(NetMulticast, Reliable)
	void SetCameraLocationAndRotation(class AKCPiece* Target);
	void SetCameraLocationAndRotation_Implementation(class AKCPiece* Target);
private:
};
