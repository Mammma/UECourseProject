// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/UECFPBaseWeapon.h"
#include "UECourseFinalProjectCharacter.h"
#include "Targets/UECFPBaseTarget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

AUECFPBaseWeapon::AUECFPBaseWeapon()
{
	// Create a gun mesh component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	WeaponMesh->SetOnlyOwnerSee(false);
	WeaponMesh->bCastDynamicShadow = false;
	WeaponMesh->CastShadow = false;
	SetRootComponent(WeaponMesh);
	
	// Create a Muzzle
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(WeaponMesh);

	// Create a fire effect from Muzzle
	FireParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("FireParticleSystemComponent");
}

void AUECFPBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUECFPBaseWeapon::IncreaseAmmo(int32 Value)
{
	AmountOfAmmo += Value;
	OnAmmoChange.Broadcast();
}

void AUECFPBaseWeapon::MakeShot()
{
	// check ammo
	if (AmountOfAmmo <= 0) return;

	const UWorld* World = GetWorld();
	if (!World) return;

	const auto Character = Cast<AUECourseFinalProjectCharacter>(GetOwner());
	if (!Character) return;

	// Determine the direction of the shot with a random scatter
	auto const HalfRad = FMath::DegreesToRadians(BulletScatter);
	FVector DirectionHit = FMath::VRandCone(Character->GetFirstPersonCameraComponent()->GetForwardVector(), HalfRad);

	// Determine the start and the end of the shot
	FVector StartVectorHit = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector EndVectorHit = StartVectorHit + DirectionHit * HitScanDistance;

	// Make Hit Scan
	FHitResult HitResult;
	if (World->LineTraceSingleByChannel(HitResult, StartVectorHit, EndVectorHit, ECC_Visibility, FCollisionQueryParams(), FCollisionResponseParams())) {
		EndVectorHit = HitResult.ImpactPoint;
		// Check if target was hit
		AUECFPBaseTarget* Target = Cast<AUECFPBaseTarget>(HitResult.Actor);
		if (Target) {
			Target->DestroyTarget();
		}
		// Try to spawn an impact effect and a decal at the hit point
		SpawnImpactEffect(HitResult);
	}

	// Try to spawn trace effect
	SpawnTraceEffect(FP_MuzzleLocation->GetComponentLocation(), EndVectorHit);

	// try and play the sound
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

	// Try to activate a fire from muzzle
	if (FireParticleSystemComponent) {
		FireParticleSystemComponent->SetWorldLocation(FP_MuzzleLocation->GetComponentLocation());
		FireParticleSystemComponent->ActivateSystem();
	}

	// Decrease by one bullet
	ReduceAmmo();
}

void AUECFPBaseWeapon::ReduceAmmo()
{
	if(AmountOfAmmo > 0)
		AmountOfAmmo -= 1;
	OnAmmoChange.Broadcast();
}

void AUECFPBaseWeapon::SpawnImpactEffect(const FHitResult& HitResult)
{
	if (auto World = GetWorld()) {
		if (ImpactEffect) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ImpactEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		}
		if (ImpactMaterialDecal) {
			UGameplayStatics::SpawnDecalAtLocation(World, ImpactMaterialDecal, ImpactDecalSize, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
		}
	}
}

void AUECFPBaseWeapon::SpawnTraceEffect(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!TraceEffect) return;

	if (auto World = GetWorld()) {
		auto Trace = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, TraceEffect, TraceStart);
		if (Trace) {
			// Set the end location to a variable created in niagra system
			Trace->SetNiagaraVariableVec3(TraceEndLocationName, TraceEnd);
		}
	}
}



