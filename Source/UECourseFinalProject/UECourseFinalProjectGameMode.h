// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UECourseFinalProjectGameMode.generated.h"

class AUECFPBaseTarget;
class AUECFPAmmo;

USTRUCT()
struct FGameData {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 1))
	int32 AmountOfTargets = 15;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0))
	int32 SpawnTargetRadius = 2000;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0))
	int32 RadiusInWhichTargetsAreCounted = 1500;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 1))
	int32 AmountOfTargetsNeedToDestroy = 10;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0))
	float IncreaseAmountOfTargetPercent = 10;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0))
	float IncreaseSpawnTargetRadiusPercent = 5;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0.01))
	float StepScaleTarget = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0.1))
	float MinScaleTarget = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Game", meta = (ClampMin = 0))
	int32 MinDistanceBetweenTargets = 80;

	UPROPERTY(EditAnywhere, Category = "Game")
	TArray<TSubclassOf<AUECFPBaseTarget>> TargetClasses;

	UPROPERTY(EditAnywhere, Category = "Game")
	TSubclassOf<AUECFPAmmo> AmmoClass;

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateGameInfo);

UCLASS(minimalapi)
class AUECourseFinalProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUECourseFinalProjectGameMode();

	/** Calling when need to update widgets */
	UPROPERTY(BlueprintAssignable)
	FOnUpdateGameInfo OnUpdateGameInfo;

	/** Finding a random location within a specific radius using the navigation mesh */
	UFUNCTION(BlueprintCallable)
	FVector GetRandomLocationInRadius(AActor* CenterActor, int32 Radius, bool IsReachablePoint);

	/** Return the number of the current wave */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWave() const { return CurrentWave; }

	/** Return the number of destroying targets */
	UFUNCTION(BlueprintCallable)
	int32 GetDestroyingTargets() const { return DestroyingTargets; }

	/** Updating info: DestroyingTargets, DestroyingRealTargets. Broadcast. Create new wave if DestroyingRealTargets == AmountOfTargetsNeedToDestroy */
	UFUNCTION(BlueprintCallable)
	void UpdateInfoTargets(AUECFPBaseTarget* Target);

	UFUNCTION(BlueprintCallable)
	void SpawnAmmo(int32 Radius);

	UFUNCTION(BlueprintCallable)
	void SpawnTargets(int32 Radius, int32 _AmountOfTargets);

	UFUNCTION(BlueprintCallable)
	bool CheckMinDistanceBetweenTargets(FVector& Location, TArray<AActor*> _Targets);

	UFUNCTION(BlueprintCallable)
	void NewWave();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, Category = "GameData")
	FGameData GameData;

private:

	TArray<AActor*> Targets;

	int32 DestroyingTargets = 0;

	int32 DestroyingRealTargets = 0;

	int32 CurrentWave = 1;

	float CurrentTagrgetScale = 1.0f;

	bool IsDestroyingAllTargets = false;
	
};



