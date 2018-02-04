// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleShipsGameInstance.generated.h"




/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API UBattleShipsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
		// Even though its not necessary for such a small scale game that doesnt even have many states or use sessions for multiplayer yet.
		// I like the idea of tracking and handling game-state stuff in this persistent game instance
		// and I think its worth getting practice using it here again in this game to get better at interweaving the States together 
		// between player controllers/gamestate/gamemode/HUD classes
		
		// Hopefully it should be less confusing than shooter games since this is technically single player.
		// At first I should just create the Tracking State variables, and set the states correctly without doing any control logic. 
		// Then once everything appears to be working correctly I might see possible improvements with the many options I have opened up by using this class.
		
		// As of right now I will create the variables only
		// TODO: Look at Fusion and how I handled the state changes then make the necessary modifications.
		// TODO: I know I set the initial state at one of the override functions, I should look at that first.
		// TODO: Afterwards i'm pretty sure I got the gameinstance from the player controllers when the game mode swapped after I loaded levels and then changed the other states from there. ("starting" states)
		// TODO: I think I changed the "leaveing" states by triggers that happened when games ended in the gamestate/gamemode?? or player controller I would need to double check


		
};
