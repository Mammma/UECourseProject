// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UECFPBaseTarget.generated.h"

class UParticleSystem;
class USphereComponent;

UCLASS()
class UECOURSEFINALPROJECT_API AUECFPBaseTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	AUECFPBaseTarget();

	//virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DestroyTarget();

	/** Does the destruction count for the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRealTarget;

protected:
	virtual void BeginPlay() override;

	/** Target Mesh */
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	/** Spawn when destroying the target */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UParticleSystem* DestroyParticleSystem;
	
};
