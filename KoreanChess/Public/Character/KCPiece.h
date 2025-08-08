// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KoreanChess.h"
#include "GameFramework/Character.h"
#include "KCPiece.generated.h"

UCLASS()
class KOREANCHESS_API AKCPiece : public ACharacter
{
	GENERATED_BODY()

public:
	AKCPiece();
	AKCPiece(const FObjectInitializer& ObjectInitializer);

	void SetAttackTarget(TWeakObjectPtr<AKCPiece> NewAttackTarget);
	void SetPiecePosition(FPos NewPosition) { CurrentPos = NewPosition; }
	int32 GetDeathScore() { return DeathScore; }
	float GetDamage() { return Damage; }
	float GetMaxHP() { return MaxHP; }
	float GetHP() { return CurrentHP; }
	float GetHpPercent();
	EPieceTeam GetTeam() { return Team; }
	EPieceState GetState() { return State; }
	EPieceKinds GetKind() { return Kind; }
	ESkillType GetSkillType() { return SkillType; }
	UFUNCTION(BlueprintCallable)
	void SetState(EPieceState NewState);
	void SetCurrentPos(FPos NewPos) { CurrentPos = NewPos; }
	FPos GetCurrentPos() { return CurrentPos; }

	void SelectThis();
	void DeSelectThis();

	virtual void ActiveStart() PURE_VIRTUAL(AKCPiece::ActiveStart, );
	virtual void ActiveDoing() PURE_VIRTUAL(AKCPiece::ActiveDoing, );
	virtual void AttackStart() PURE_VIRTUAL(AKCPiece::Attack, );
	virtual void Attacking() PURE_VIRTUAL(AKCPiece::Attacking, );
	virtual void ActiveEnd();
	virtual void AttackEnd();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath();
	void SelectButtonClicked();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ClickedCapsule();
	UFUNCTION()
	void BeginOverCapsule();
	UFUNCTION()
	void OutOverCapsule();

	void ChangeColor(EColorType NewColor);

	UFUNCTION(NetMulticast, Reliable)
	void PlayEffect(UAnimMontage* AnimMontage);
	void PlayEffect_Implementation(UAnimMontage* AnimMontage);

	void UpdateParticle();
	UFUNCTION(BlueprintCallable)
	void InitializeRotation();

	UFUNCTION()
	void OnRep_ChangeHP();
protected:
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UParticleSystem* DeathEffect;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UParticleSystem* SpawnEffect;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class USoundCue* DeathEffectSound;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UMaterialInterface* BlueMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UMaterialInterface* RedMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UMaterialInterface* GreenMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Effect)
	class UMaterialInterface* BlackMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> HPBarWB;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> ActionSelectWB;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* CanAttackParticle;
	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* OutLineMesh;
	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* HPWidget;
	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* SelectWidget;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxHP;
	UPROPERTY(ReplicatedUsing = OnRep_ChangeHP)
	float CurrentHP;
	UPROPERTY(EditAnywhere, Category = "Status", Replicated)
	int32 CoolTime;
	UPROPERTY(EditAnywhere, Category = "Status")
	int32 DeathScore;
	UPROPERTY(EditAnywhere, Category = "Status")
	float Damage;
	UPROPERTY(EditAnywhere, Category = "PieceInfo")
	EPieceTeam Team;
	UPROPERTY(EditAnywhere, Category = "PieceInfo")
	EPieceKinds Kind;
	UPROPERTY(EditAnywhere, Category = "PieceInfo", Replicated)
	EPieceState State;
	UPROPERTY(EditAnywhere, Category = "PieceInfo", Replicated)
	ESkillType SkillType;
	UPROPERTY()
	FPos CurrentPos;
	EColorType CurrentColorType = EColorType::BLACK;

	//This Filled Only Server
	TWeakObjectPtr<AKCPiece> AttackTarget;
	//This Filled Only when Character dead
	TWeakObjectPtr<class APieceController> Instigator;
};
