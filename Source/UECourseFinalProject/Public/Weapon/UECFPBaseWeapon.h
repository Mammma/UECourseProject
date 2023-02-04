// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UECFPBaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoChange);

class USceneComponent;
class UAnimMontage;
class UParticleSystemComponent;
class USoundBase;
class UNiagaraSystem;


UCLASS()
class UECOURSEFINALPROJECT_API AUECFPBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Delegate to call when ammo changes */
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChange OnAmmoChange;

	UFUNCTION(BlueprintCallable)
	void IncreaseAmmo(int32 Value);

	/** Decrease by one bullet */
	UFUNCTION(BlueprintCallable)
	void ReduceAmmo();

	/** Return current ammo */
	UFUNCTION(BlueprintCallable)
	int32 GetAmountOfAmmo() const { return AmountOfAmmo; };

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

	AUECFPBaseWeapon();

	/** Make Shot. */
	void MakeShot();

protected:
	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USceneComponent* FP_MuzzleLocation;

	/** Current ammo */
	UPROPERTY(EditAnywhere, Category = "Ammo", meta = (ClampMin = 0))
	int32 AmountOfAmmo = 20;

	/** Sound to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireAnimation;

	/** Shot distance */
	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (ClampMin = 1, ClampMax = 2500))
	int32 HitScanDistance = 1500;

	/** Half the radius of possible bullet scatter */
	UPROPERTY(EditAnywhere, Category = "Weapon Settings", meta = (ClampMin = 1, ClampMax = 3))
	float BulletScatter = 1.0f;

	/** ParticleSystemComponent to activate a fire from muzzle */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UParticleSystemComponent* FireParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceEffect;

	/** Name of a variable created in niagra system */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceEndLocationName = "TraceEndLocation";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* ImpactMaterialDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FVector ImpactDecalSize = FVector(10.0f);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/* Spawn an impact effect and a decal at the hit point */
	void SpawnImpactEffect(const FHitResult& HitResult);

	/* Spawn Trace Effect from the shot */
	void SpawnTraceEffect(const FVector& TraceStart, const FVector& TraceEnd);
};
