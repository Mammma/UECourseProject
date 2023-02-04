// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UECFPAmmoWidget.generated.h"

class UTextBlock;
class AUECourseFinalProjectCharacter;

UCLASS()
class UECOURSEFINALPROJECT_API UUECFPAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

	AUECourseFinalProjectCharacter* Player;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Update textblock when the number of ammo has changed */
	UFUNCTION()
	void OnAmmoChange();

	/** Text block to view the current amount of ammo */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextBlockAmountOfAmmo;	
};
