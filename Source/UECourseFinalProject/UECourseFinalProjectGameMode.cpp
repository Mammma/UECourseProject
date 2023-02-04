// Copyright Epic Games, Inc. All Rights Reserved.

#include "UECourseFinalProjectGameMode.h"
#include "UECourseFinalProjectHUD.h"
#include "UECourseFinalProjectCharacter.h"
#include "Targets/UECFPBaseTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup/UECFPAmmo.h"
#include "NavigationSystem.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPGameMode, All, All);

AUECourseFinalProjectGameMode::AUECourseFinalProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUECourseFinalProjectHUD::StaticClass();
}

void AUECourseFinalProjectGameMode::BeginPlay()
{
	// Spawn the required number of targets for the wave to pass
	SpawnTargets(GameData.RadiusInWhichTargetsAreCounted, GameData.AmountOfTargetsNeedToDestroy);
	// Spawn remaining targets
	SpawnTargets(GameData.SpawnTargetRadius, GameData.AmountOfTargets- GameData.AmountOfTargetsNeedToDestroy);
}

bool AUECourseFinalProjectGameMode::CheckMinDistanceBetweenTargets(FVector& Location, TArray<AActor*> _Targets)
{
	for (auto Target : _Targets) {
		float Distance = FVector::Distance(Location, Target->GetActorLocation());
		if (Distance <= GameData.MinDistanceBetweenTargets) {
			UE_LOG(LogGameMode, Warning, TEXT("Min Distance between Targets: %f"), Distance);
			return false;
		}
	}
	return true;
}

FVector AUECourseFinalProjectGameMode::GetRandomLocationInRadius(AActor* CenterActor, int32 Radius, bool IsReachablePoint)
{
	const auto NavSystem = UNavigationSystemV1::GetCurrent(CenterActor);
	if (NavSystem) {
		FNavLocation NavLocation;

		bool Found;
		if (!IsReachablePoint) {
			Found = NavSystem->GetRandomPointInNavigableRadius(CenterActor->GetActorLocation(), Radius, NavLocation);
		}
		else {
			Found = NavSystem->GetRandomReachablePointInRadius(CenterActor->GetActorLocation(), Radius, NavLocation);
		}

		if (Found) {
			return NavLocation.Location;
		}
	}
	return FVector::ZeroVector;
}

void AUECourseFinalProjectGameMode::NewWave()
{
	// Reset variables
	CurrentTagrgetScale = 1.0f;
	DestroyingRealTargets = 0;

	// Tag - don't call to update the info when destroying targets
	IsDestroyingAllTargets = true;
	for (auto Target : Targets) {
		AUECFPBaseTarget* BaseTarget = Cast<AUECFPBaseTarget>(Target);
		BaseTarget->DestroyTarget();
	}
	Targets.Empty();
	IsDestroyingAllTargets = false;

	// Increase the number of targets and spawn radius by a percentage
	GameData.AmountOfTargets += GameData.AmountOfTargets * GameData.IncreaseAmountOfTargetPercent / 100.0f;
	GameData.AmountOfTargetsNeedToDestroy += GameData.AmountOfTargetsNeedToDestroy * GameData.IncreaseAmountOfTargetPercent / 100.0f;
	GameData.SpawnTargetRadius += GameData.SpawnTargetRadius * GameData.IncreaseSpawnTargetRadiusPercent / 100.0f;
	GameData.RadiusInWhichTargetsAreCounted += GameData.RadiusInWhichTargetsAreCounted * GameData.IncreaseSpawnTargetRadiusPercent / 100.0f;

	// New spawn
	SpawnTargets(GameData.SpawnTargetRadius, GameData.AmountOfTargets - GameData.AmountOfTargetsNeedToDestroy);
	SpawnTargets(GameData.RadiusInWhichTargetsAreCounted, GameData.AmountOfTargetsNeedToDestroy);
	SpawnAmmo(GameData.SpawnTargetRadius);

	CurrentWave++;

	OnUpdateGameInfo.Broadcast();
}


void AUECourseFinalProjectGameMode::SpawnAmmo(int32 Radius)
{
	const auto World = GetWorld();
	if (!World) return;
	const auto Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Pawn) return;

	//	Set Spawn Parameters
	FActorSpawnParameters AmmoSpawnInfo;
	AmmoSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//	Get random location
	FVector AmmoSpawnLocation = GetRandomLocationInRadius(Pawn, Radius, true);
	if (AmmoSpawnLocation.IsZero()) {
		AmmoSpawnLocation = Pawn->GetActorLocation() + Pawn->GetActorForwardVector() * 300;
	}
	FTransform AmmoTransform = FTransform(FRotator::ZeroRotator, AmmoSpawnLocation , FVector::OneVector);

	GetWorld()->SpawnActor<AUECFPAmmo>(GameData.AmmoClass, AmmoTransform, AmmoSpawnInfo);
}

void AUECourseFinalProjectGameMode::SpawnTargets(int32 Radius, int32 _AmountOfTargets)
{
	// Сhecking for target types
	if (GameData.TargetClasses.Num() <= 0) return;

	const auto World = GetWorld();
	if (!World) return;
	const auto Pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!Pawn) return;

	//Since it is possible that there will be no place for actors to spawn, we limit the number of attempts to spawn
	int32 FactorMaxTrySpawn = 10;
	int32 CurrentAmountOfTargets = 0;
	for (int i = 0; i < _AmountOfTargets * FactorMaxTrySpawn; i++) {
		FVector SpawnTargetLocation = GetRandomLocationInRadius(Pawn, Radius, false);
		if (!CheckMinDistanceBetweenTargets(SpawnTargetLocation, Targets)) {
			continue;
		}

		FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnTargetLocation, FVector(CurrentTagrgetScale));
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		//Set random target type
		auto TargetClass = GameData.TargetClasses[FMath::FRandRange(0, GameData.TargetClasses.Num())];

		auto Target = GetWorld()->SpawnActor<AUECFPBaseTarget>(TargetClass, SpawnTransform, SpawnInfo);
		if (Target) {
			// Determine if the target is within RadiusInWhichTargetsAreCounted
			if (FVector::Distance(Pawn->GetActorLocation(), Target->GetActorLocation()) <= GameData.RadiusInWhichTargetsAreCounted) {
				Target->IsRealTarget = true;
			}
			// Changing the scale of the target each step
			CurrentTagrgetScale -= GameData.StepScaleTarget;
			if (CurrentTagrgetScale < GameData.MinScaleTarget) {
				CurrentTagrgetScale = GameData.MinScaleTarget;
			}

			CurrentAmountOfTargets++;
			Targets.Add(Target);
		}
		if (CurrentAmountOfTargets >= _AmountOfTargets) break;
	}
}

void AUECourseFinalProjectGameMode::UpdateInfoTargets(AUECFPBaseTarget* Target)
{
	if (IsDestroyingAllTargets) return;
	
	DestroyingTargets++;
	if (Target->IsRealTarget) {
		DestroyingRealTargets++;
	}

	OnUpdateGameInfo.Broadcast();

	if (DestroyingRealTargets == GameData.AmountOfTargetsNeedToDestroy) {
		NewWave();
	}
}
