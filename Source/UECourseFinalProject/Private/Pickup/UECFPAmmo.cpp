// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup/UECFPAmmo.h"
#include "Weapon/UECFPBaseWeapon.h"
#include "Components/SphereComponent.h"
#include "../UECourseFinalProjectCharacter.h"

AUECFPAmmo::AUECFPAmmo()
{
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Ammo"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(SphereComponent);
}

void AUECFPAmmo::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AUECFPAmmo::OnActorBeginOverlapHandler);
}

void AUECFPAmmo::OnActorBeginOverlapHandler(AActor* OverlappedActor, AActor* OtherActor)
{
	auto Player = Cast<AUECourseFinalProjectCharacter>(OtherActor);
	if (Player) {
		Player->GetWeapon()->IncreaseAmmo(AmmountOfAmmo);
	}
	Destroy();
}

// Called every frame
void AUECFPAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate actor
	float RandomYaw = FMath::FRandRange(1.0f, 3.0f);
	AddActorLocalRotation(FRotator(0.0f, RandomYaw, 0.0f));
}

