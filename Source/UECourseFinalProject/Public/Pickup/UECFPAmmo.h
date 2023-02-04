// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UECFPAmmo.generated.h"

UCLASS()
class UECOURSEFINALPROJECT_API AUECFPAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	AUECFPAmmo();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/** Ammo Mesh*/
	UPROPERTY(EditAnywhere, Category=Components)
	UStaticMeshComponent* MeshComponent;

	/** When overlapping, increase the player's ammo and destroy it */
	UFUNCTION()
	void OnActorBeginOverlapHandler(AActor* OverlappedActor, AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", meta = (ClampMin = 0))
	int32 AmmountOfAmmo = 20;

};
