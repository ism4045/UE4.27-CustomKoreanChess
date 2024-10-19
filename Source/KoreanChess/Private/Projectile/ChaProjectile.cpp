// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ChaProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Character/KCPiece.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Character/MaPiece.h"
#include "Character/PoPiece.h"
#include "Character/SangPiece.h"

// Sets default values
AChaProjectile::AChaProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(37.5f);
	RootComponent = SphereComponent;

	ProjectileEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileEffect"));
	ProjectileEffect->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	if (GetLocalRole() == ROLE_Authority) {
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AChaProjectile::OnProjectileImpact);
	}
}

// Called when the game starts or when spawned
void AChaProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChaProjectile::Destroyed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(), FVector(0.5f, 0.5f, 0.5f));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

void AChaProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AttackTarget == nullptr)
		return;
	if (Cast<AKCPiece>(OtherActor) == AttackTarget.Get()) {
		FDamageEvent DamageEvent;
		int32 FinalDamage = Cast<AKCPiece>(GetOwner())->GetDamage();
		if (Cast<AKCPiece>(OtherActor)->GetKind() == EPieceKinds::CHA) {
			int32 Result = FMath::RandRange(1, 100);
			if (Result <= 50) {
				FinalDamage *= 2;
			}
		}
		else if (Cast<AKCPiece>(OtherActor)->GetKind() == EPieceKinds::MA) {
			Cast<AMaPiece>(GetOwner())->ReCoveryHP(Cast<AKCPiece>(GetOwner())->GetDamage() * 0.2);
		}
		else if (Cast<AKCPiece>(OtherActor)->GetKind() == EPieceKinds::SANG) {
			Cast<ASangPiece>(GetOwner())->ReCoveryHP(Cast<AKCPiece>(GetOwner())->GetDamage() * 0.2);
		}
		UGameplayStatics::ApplyDamage(AttackTarget.Get(), FinalDamage, Cast<AKCPiece>(GetOwner())->GetController(), this, UDamageType::StaticClass());

		if (Cast<AKCPiece>(OtherActor)->GetKind() == EPieceKinds::JANG) {
			if (FinalDamage > Cast<AKCPiece>(OtherActor)->GetHP()) {
				Cast<APoPiece>(GetOwner())->IncreaseDamage();
			}
		}

		Destroy();
	}
}

// Called every frame
void AChaProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

