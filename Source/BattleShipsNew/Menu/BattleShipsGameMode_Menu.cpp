// @Maxpro 2018

#include "BattleShipsGameMode_Menu.h"

#include "BattleShipsPlayerController_Menu.h"
#include "../BattleShipsNewHUD.h"

ABattleShipsGameMode_Menu::ABattleShipsGameMode_Menu()
{

	// Here we want to use the PlayerController_Menu  sub class that we will create
	PlayerControllerClass = ABattleShipsPlayerController_Menu::StaticClass();
	HUDClass = ABattleShipsNewHUD::StaticClass(); 

	// pretty sure the 2 classes above are not being used since inside of the GameModeMenu_BP we choose the blueprint ones.


}


void ABattleShipsGameMode_Menu::RestartPlayer(AController * NewPlayer)
{

	// NOTE: Overriding and not calling super since we dont need player start or pawn in the menu here.


}

