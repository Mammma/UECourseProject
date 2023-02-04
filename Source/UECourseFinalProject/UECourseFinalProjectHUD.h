// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UECourseFinalProjectHUD.generated.h"

UCLASS()
class AUECourseFinalProjectHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUECourseFinalProjectHUD();

	void BeginPlay() override;
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

protected:
	/**  Widget to display the current amount of ammo */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> AmmoWidgetClass;
	/** Widget to display statistics of destroyed targets and the current wave */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> GameInfoWidgetClass;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

