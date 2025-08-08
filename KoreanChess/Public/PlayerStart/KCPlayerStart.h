// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/PlayerStart.h"
#include "KCPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class KOREANCHESS_API AKCPlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	EPieceTeam GetTeam() { return Team; }
public:
	UPROPERTY(EditAnywhere)
	EPieceTeam Team;
};
