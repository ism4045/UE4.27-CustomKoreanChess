// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/Actor.h"
#include "Character/KCPiece.h"
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
	AKCPiece* SpawnSpecificActor(EPieceKinds Kind, EPieceTeam Team, FVector SpawnLocation);
	void SpawnDeathPiece(EPieceKinds Kind, EPieceTeam Team);
	FPointMap InitializeBoard();
	void InitializeDeathBoard();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Jol_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteJol;
	UPROPERTY(EditDefaultsOnly, Category = "Jol_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackJol;
	UPROPERTY(EditDefaultsOnly, Category = "Cha_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteCha;
	UPROPERTY(EditDefaultsOnly, Category = "Cha_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackCha;
	UPROPERTY(EditDefaultsOnly, Category = "Ma_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteMa;
	UPROPERTY(EditDefaultsOnly, Category = "Ma_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackMa;
	UPROPERTY(EditDefaultsOnly, Category = "Sang_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteSang;
	UPROPERTY(EditDefaultsOnly, Category = "Sang_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackSang;
	UPROPERTY(EditDefaultsOnly, Category = "Po_Pieces")
	TSubclassOf<class AKCPiece> BP_WhitePo;
	UPROPERTY(EditDefaultsOnly, Category = "Po_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackPo;
	UPROPERTY(EditDefaultsOnly, Category = "Jang_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteJang;
	UPROPERTY(EditDefaultsOnly, Category = "Jang_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackJang;
	UPROPERTY(EditDefaultsOnly, Category = "Sa_Pieces")
	TSubclassOf<class AKCPiece> BP_WhiteSa;
	UPROPERTY(EditDefaultsOnly, Category = "Sa_Pieces")
	TSubclassOf<class AKCPiece> BP_BlackSa;

	UPROPERTY(EditDefaultsOnly, Category = "PointActor")
	TSubclassOf<class AKCPointActor> BP_PointActor;
	UPROPERTY(EditDefaultsOnly, Category = "PointActor")
	TSubclassOf<class AKCDeathBoardActor> BP_DeathBoardActor;

	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> BlackDeathBoard;
	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCDeathBoardActor>> WhiteDeathBoard;
};
