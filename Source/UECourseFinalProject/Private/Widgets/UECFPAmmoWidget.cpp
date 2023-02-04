// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UECFPAmmoWidget.h"
#include "Weapon/UECFPBaseWeapon.h"
#include "Components/TextBlock.h"
#include "../UECourseFinalProjectCharacter.h"

void UUECFPAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlockAmountOfAmmo = (UTextBlock*)GetWidgetFromName(TEXT("TextBlockAmountOfAmmo"));

	auto PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn) return;
	Player = Cast<AUECourseFinalProjectCharacter>(PlayerPawn);
	auto Weapon = Player->GetWeapon();
	if (Weapon && TextBlockAmountOfAmmo) {
		TextBlockAmountOfAmmo->SetText(FText::AsNumber(Weapon->GetAmountOfAmmo()));
		Weapon->OnAmmoChange.AddDynamic(this, &UUECFPAmmoWidget::OnAmmoChange);
	}	
}

void UUECFPAmmoWidget::NativeDestruct()
{
	if (Player) {
		if (auto Weapon = Player->GetWeapon())
			Weapon->OnAmmoChange.RemoveDynamic(this, &UUECFPAmmoWidget::OnAmmoChange);
	}
	Super::NativeDestruct();
}

void UUECFPAmmoWidget::OnAmmoChange()
{
	if (TextBlockAmountOfAmmo) {
		if (auto Weapon = Player->GetWeapon())
			TextBlockAmountOfAmmo->SetText(FText::AsNumber(Weapon->GetAmountOfAmmo()));
	}
}

