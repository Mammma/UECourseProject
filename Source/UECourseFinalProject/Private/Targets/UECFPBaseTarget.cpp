// Fill out your copyright notice in the Description page of Project Settings.


#include "Targets/UECFPBaseTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "../UECourseFinalProjectGameMode.h"


AUECFPBaseTarget::AUECFPBaseTarget()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(MeshComponent);
}

void AUECFPBaseTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AUECFPBaseTarget::DestroyTarget()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyParticleSystem, GetActorTransform(), true);
	auto const GameMode = Cast<AUECourseFinalProjectGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		GameMode->UpdateInfoTargets(this);
	}
	Destroy();
}
