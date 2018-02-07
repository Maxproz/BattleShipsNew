// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleShipsNewHUD.generated.h"


class UMainMenuWidget;


// Probably will be using one HUD class to control all game widgets and just re-load the same one/widgets
// when loading maps and menus and the specific player controller will control what widgets are shown.

// NOTE: We will probably want the HUD class to send and recieve messeges with the game state / player state 
// to broadcast hits and misses.

// But for that to work cleanly we will want an enum here to make sure we are only sending these messeges while we
// are inside gameplay.
UENUM(BlueprintType)	
enum class EHUDStateEnum : uint8
{
	HS_Menu 	UMETA(DisplayName = "Menu"),
	HS_InGame 	UMETA(DisplayName = "InGame"),
	HS_Error 	UMETA(DisplayName = "Error")
};


UCLASS()
class ABattleShipsNewHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABattleShipsNewHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	FORCEINLINE EHUDStateEnum GetCurrentHUDState() const { return CurrentHUDState; }
	FORCEINLINE void SetCurrentHUDState(const EHUDStateEnum& InNewState) { CurrentHUDState = InNewState; }

	FString GetHUDStateEnumAsString(uint8 EnumValue);

	FORCEINLINE UMainMenuWidget* GetMainMenuWidget() const { return ActiveMainMenuWidget; }


	void CreateGameWidgets();
	void RemoveGameWidgets();

	void ShowMainMenu();
	void HideMainMenu();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	EHUDStateEnum CurrentHUDState = EHUDStateEnum::HS_Error;


	UPROPERTY(EditDefaultsOnly, Category = "Menu Widgets")
	TAssetSubclassOf<UMainMenuWidget> MainMenuWidget;


	UMainMenuWidget* ActiveMainMenuWidget;

	void CreateMainMenuWidget();

private:

	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

