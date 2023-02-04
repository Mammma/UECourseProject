// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UECFPGameProgressWidget.h"
#include "Components/TextBlock.h"
#include "../UECourseFinalProjectGameMode.h"

void UUECFPGameProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlockWave = (UTextBlock*)GetWidgetFromName(TEXT("TextBlockWave"));
	TextBlockScore = (UTextBlock*)GetWidgetFromName(TEXT("TextBlockScore"));

	auto GameModeBase = GetWorld()->GetAuthGameMode();
	if (!GameModeBase) return;
	GameMode = Cast<AUECourseFinalProjectGameMode>(GameModeBase);
	if (GameMode) {
		if(TextBlockWave)
			TextBlockWave->SetText(FText::AsNumber(GameMode->GetCurrentWave()));

		if (TextBlockScore)
			TextBlockScore->SetText(FText::AsNumber(GameMode->GetDestroyingTargets()));

		GameMode->OnUpdateGameInfo.AddDynamic(this, &UUECFPGameProgressWidget::OnGameInfoChange);
	}
}

void UUECFPGameProgressWidget::NativeDestruct()
{
	if (GameMode) {
		GameMode->OnUpdateGameInfo.RemoveDynamic(this, &UUECFPGameProgressWidget::OnGameInfoChange);
	}
	Super::NativeDestruct();
}

void UUECFPGameProgressWidget::OnGameInfoChange()
{
	if (!GameMode) return;

	if (TextBlockWave)
		TextBlockWave->SetText(FText::AsNumber(GameMode->GetCurrentWave()));
	if (TextBlockScore)
		TextBlockScore->SetText(FText::AsNumber(GameMode->GetDestroyingTargets()));
}
