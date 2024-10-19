// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/NetSerialization.h"
#include "KCData.generated.h"

UENUM()
enum class ESkillType : uint8
{
	PASSIVE,
	ACTIVE,
};

UENUM()
enum class EActionMessageType : uint8
{
	MOVEORATTACK,
	ACTIVESKILL,
};

UENUM()
enum class EColorType : uint8
{
	RED,
	GREEN,
	BLUE,
	BLACK,
};

UENUM()
enum class EActionInTurn : uint8
{
	MOVE,
	ATTACK,
	DEAD,
	ACTIVESKILL,
};

UENUM()
enum class EMessageType : uint8
{
	READY,
	GAMESTART,
	SPAWN,
	SELECT,
	MOVE,
	ATTACK,
	GAMEOVER,
	SUCCESS,
	CANTINPUT,
	TURNEND,
	ACCEPT,
	DECLINE,
	CINEMA,
};

UENUM()
enum class EPointState : uint8
{
	NONE,
	CANMOVE,
	CANSPAWN
};

UENUM()
enum class EPieceTeam : uint8
{
	WHITE,
	BLACK
};

UENUM()
enum class EPieceKinds : uint8
{
	JOL,
	SA,
	SANG,
	MA,
	PO,
	CHA,
	JANG
};

UENUM()
enum class EPieceState : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	DEAD,
	CANDAMAGED,
	CANSKILLDAMAGED,
	ACTIVESKILL,
};

USTRUCT()
struct FPos
{
	GENERATED_BODY()
public:
	FPos(int64 NewX, int64 NewY) { PosX = NewX; PosY = NewY; }
	FPos() { PosX = 0; PosY = 0; }
	bool operator==(const FPos& other) const {
		return PosX == other.PosX && PosY == other.PosY;
	}
	FPos operator+(FPos& other) {
		return (FPos(PosX + other.PosX, PosY + other.PosY));
	}
	FPos operator-(FPos& other) {
		return (FPos(PosX - other.PosX, PosY - other.PosY));
	}

public:
	UPROPERTY(EditAnywhere)
	int64 PosX;
	UPROPERTY(EditAnywhere)
	int64 PosY;
};

USTRUCT()
struct FMAResultData
{
	GENERATED_BODY()
public:
	bool IsEmpty() { return CanMovePointArray.Num() == 0 && CanAttackActorArray.Num() == 0; }
public:
	TArray<TWeakObjectPtr<class AKCPointActor>> CanMovePointArray;
	TArray<TWeakObjectPtr<class AKCPiece>> CanAttackActorArray;
};

USTRUCT()
struct FBoardPointInfo
{
	GENERATED_BODY()
public:
	FBoardPointInfo();
	FBoardPointInfo(FPos NewPosition, EPointState NewState = EPointState::NONE);
	FBoardPointInfo(int64 NewPosX, int64 NewPosY, EPointState NewState = EPointState::NONE);

	void SetPosition(int64 NewPosX, int64 NewPosY);
	void SetPosition(FPos NewPosition);
	void SetState(EPointState NewState);
public:
	UPROPERTY(EditAnywhere)
	FPos Position;
	UPROPERTY(EditAnywhere)
	EPointState State;
};

USTRUCT()
struct FPointMap
{
	GENERATED_BODY()
public:
	FPointMap();
	FPointMap(TArray<FPos> NewPosArr, TArray<TWeakObjectPtr<class AKCPointActor>> NewPieceArr);
	
	void Add(FPos NewPos, TWeakObjectPtr<class AKCPointActor> NewPiece);
	bool Contains(FPos CurrentPos);
	TWeakObjectPtr<class AKCPointActor> operator[](FPos Key);
public:
	UPROPERTY()
	TArray<FPos> Pos;
	UPROPERTY()
	TArray<TWeakObjectPtr<class AKCPointActor>> Piece;
};

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FPos& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FPos& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FPos));
	return Hash;
}
#endif