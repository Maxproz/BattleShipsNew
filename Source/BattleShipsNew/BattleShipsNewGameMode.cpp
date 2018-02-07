// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BattleShipsNewGameMode.h"
#include "BattleShipsNewHUD.h"
#include "BattleShipsNewCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABattleShipsNewGameMode::ABattleShipsNewGameMode()
	: Super()
{


	// NOTE: Not sure what I will use this for yet, or if I even ever will. 
	// The gameplay gamemode and menu one are handled in their seperate folders and they are not inheriting from this one.



	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/UnTracked/FirstPersonDefaultChar/FirstPerson/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	//// use our custom HUD class
	//HUDClass = ABattleShipsNewHUD::StaticClass();



}
