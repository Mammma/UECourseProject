// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UECFPGameProgressWidget.generated.h"


class UTextBlock;
class AUECourseFinalProjectGameMode;

UCLASS()
class UECOURSEFINALPROJECT_API UUECFPGameProgressWidget : public UUserWidget
{
	GENERATED_BODY()

	AUECourseFinalProjectGameMode* GameMode;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Update textblock when the number of destroyed targets or wave have been changed */
	UFUNCTION()
	void OnGameInfoChange();

	/** Text block to view the current wave */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextBlockWave;

	/** Text block to view the number of destroyed targets */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextBlockScore;
	
};
