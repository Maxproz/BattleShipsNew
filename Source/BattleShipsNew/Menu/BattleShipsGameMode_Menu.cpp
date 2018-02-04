// @Maxpro 2018

#include "BattleShipsGameMode_Menu.h"

#include "BattleShipsPlayerController_Menu.h"

ABattleShipsGameMode_Menu::ABattleShipsGameMode_Menu()
{

	// Here we want to use the PlayerController_Menu  sub class that we will create
	PlayerControllerClass = ABattleShipsPlayerController_Menu::StaticClass();

}


void ABattleShipsGameMode_Menu::RestartPlayer(AController * NewPlayer)
{

	// NOTE: Overriding and not calling super since we dont need player start or pawn in the menu here.


}

