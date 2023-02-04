// Copyright Epic Games, Inc. All Rights Reserved.

#include "UECourseFinalProjectHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

AUECourseFinalProjectHUD::AUECourseFinalProjectHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AUECourseFinalProjectHUD::BeginPlay()
{
	Super::BeginPlay();
	// Adding widgets to the viewport
	const auto World = GetWorld();
	if (!World) return;

	auto AmmoWidget = CreateWidget<UUserWidget>(World, AmmoWidgetClass);
	if (AmmoWidget) {
		AmmoWidget->AddToViewport();
	}

	auto GameInfoWidget = CreateWidget<UUserWidget>(World, GameInfoWidgetClass);
	if (GameInfoWidget) {
		GameInfoWidget->AddToViewport();
	}
}


void AUECourseFinalProjectHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X - 10.0f),
										   (Center.Y - 10.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
