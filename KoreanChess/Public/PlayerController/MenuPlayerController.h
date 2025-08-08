// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
};
