// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BattleShipsNewHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

#include "BattleShipsGameInstance.h"
#include "Menu/BattleShipsPlayerController_Menu.h"

#include "Widgets/Menu/MainMenuWidget.h"

#include "Runtime/CoreUObject/Public/UObject/Package.h"

ABattleShipsNewHUD::ABattleShipsNewHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/UnTracked/FirstPersonDefaultChar/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

FString ABattleShipsNewHUD::GetHUDStateEnumAsString(uint8 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHUDStateEnum"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameStringByValue((int64)EnumValue); // for EnumValue == VE_Dance returns "VE_Dance"
}

void ABattleShipsNewHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void ABattleShipsNewHUD::CreateGameWidgets()
{
	CreateMainMenuWidget();

	// TODO: Future widgets...

}

void ABattleShipsNewHUD::RemoveGameWidgets()
{
	// don't really need since they are usually destroyed at the ideal moments anyway...
	// maybe it will be useful in the future.
}



void ABattleShipsNewHUD::CreateMainMenuWidget()
{

	ABattleShipsPlayerController_Menu* MPC = Cast<ABattleShipsPlayerController_Menu>(GetOwningPlayerController());
	if (!MPC) return;

	if (ActiveMainMenuWidget != nullptr) return;
	ActiveMainMenuWidget = CreateWidget<UMainMenuWidget>(GetOwningPlayerController(), MainMenuWidget.LoadSynchronous());

	// TODO: I see no reason to have a game instance ref variable... yet
	// ActiveMainMenuWidget->GameInstanceRef = Cast<UBattleShipsGameInstance>(MPC->GetGameInstance());
	ActiveMainMenuWidget->AddToViewport(0);
	ActiveMainMenuWidget->SetVisibility(ESlateVisibility::Hidden);

}

void ABattleShipsNewHUD::ShowMainMenu() { GetMainMenuWidget()->ShowWidget(); }
void ABattleShipsNewHUD::HideMainMenu() { GetMainMenuWidget()->HideWidget(); }