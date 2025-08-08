// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/Actor.h"
#include "Character/KCPiece.h"
#include "Engine/StreamableManager.h"
#include "KCSpawnManager.generated.h"

UCLASS()
class KOREANCHESS_API AKCSpawnManager : public AActor
{
	GENERATED_BODY()
	friend class AKoreanChessGameMode;
public:	
	// Sets default values for this actor's properties
	AKCSpawnManager();
protected:
	void InitializePieces(FPointMap Table);
	void InitializeDeathBoard();
	void SpawnDeathPiece(EPieceKinds Kind, EPieceTeam Team);
	AKCPiece* SpawnSpecificActor(EPieceKinds Kind, EPieceTeam Team, FVector SpawnLocation);
	FPointMap InitializeBoard();

private:
	AActor* CustomSpawnActor(TSoftClassPtr<class AActor> InObject, const FVector& InLocation, const FRotator& InRotation, const FActorSpawnParameters InSpawnParam);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Jol_Pieces")
	TSoftClassPtr<AActor> BP_WhiteJol;

	UPROPERTY(EditDefaultsOnly, Category = "Jol_Pieces")
	TSoftClassPtr<AActor> BP_BlackJol;

	UPROPERTY(EditDefaultsOnly, Category = "Cha_Pieces")
	TSoftClassPtr<AActor> BP_WhiteCha;

	UPROPERTY(EditDefaultsOnly, Category = "Cha_Pieces")
	TSoftClassPtr<AActor> BP_BlackCha;

	UPROPERTY(EditDefaultsOnly, Category = "Ma_Pieces")
	TSoftClassPtr<AActor> BP_WhiteMa;

	UPROPERTY(EditDefaultsOnly, Category = "Ma_Pieces")
	TSoftClassPtr<AActor> BP_BlackMa;

	UPROPERTY(EditDefaultsOnly, Category = "Sang_Pieces")
	TSoftClassPtr<AActor> BP_WhiteSang;

	UPROPERTY(EditDefaultsOnly, Category = "Sang_Pieces")
	TSoftClassPtr<AActor> BP_BlackSang;

	UPROPERTY(EditDefaultsOnly, Category = "Po_Pieces")
	TSoftClassPtr<AActor> BP_WhitePo;

	UPROPERTY(EditDefaultsOnly, Category = "Po_Pieces")
	TSoftClassPtr<AActor> BP_BlackPo;

	UPROPERTY(EditDefaultsOnly, Category = "Jang_Pieces")
	TSoftClassPtr<AActor> BP_WhiteJang;

	UPROPERTY(EditDefaultsOnly, Category = "Jang_Pieces")
	TSoftClassPtr<AActor> BP_BlackJang;

	UPROPERTY(EditDefaultsOnly, Category = "Sa_Pieces")
	TSoftClassPtr<AActor> BP_WhiteSa;

	UPROPERTY(EditDefaultsOnly, Category = "Sa_Pieces")
	TSoftClassPtr<AActor> BP_BlackSa;

	UPROPERTY(EditDefaultsOnly, Category = "PointActor")
	TSubclassOf<AActor> BP_PointActor;

	UPROPERTY(EditDefaultsOnly, Category = "PointActor")
	TSubclassOf<AActor> BP_DeathBoardActor;

	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard;
	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard;

	FStreamableManager StreamableManager;
	TSharedPtr<FStreamableHandle> Handle;
};
