// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "BattleShipsNewCharacter.h"
#include "BattleShipsCommander.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ABattleShipsCommander : public ABattleShipsNewCharacter
{
	GENERATED_BODY()
	
private:


	
public:

	bool bHasRequestedShipTransfer = false;
	bool bIsValidTransferLocation = false;
	
	// TODO: How will we handle animations?
	void PlayShipTransferAnimation(/* UAnimation???? */);

	// Commander should be able to open and read a live-updating map of the enemys board by pressing a binded input component key("X for example")
	// This will probably be a seperate class pointer that is instantiated inside of this class somewhere after the game has begun
	// UClass* EnemeyLiveUpdateMap;
	// This map will have a slight red glow around the border/back of it, giving the impression you are looking at the enemy.
	// UParticleSystem* RedMapGlowEffect;

	// The board squares that are chosen to be fired at when selecting squares on the enemy board will need to be tracked so we can add a X or O
	// to the mini-board after we know if its a hit or miss.
	// TMatrix<10,10> ChosenFireLocation; Need a different way to track this since the board is tracked by A-J on one side of the matrix not both by ints?

	// As of right now I am thinking that the player will need to be actively tracking the damage done to his own ships so he knows which ones are safe to travel to if he needs to.
	// And also so he knows the state of the game.
	// Because of this I am thinking that the player will only recieve the global broadcasts about hits/misses/sinks and will not have an "always active" view of his own side gameboard he can bring up.
	// As a result I will give the player a "Telescope" class that will be binded to another input component "F" or something that he can use to observe the enemy fire coming in to his ships from the distance.
	// Even though this is more of a multiplayer kind of feature I want to feel the immersion it brings if any, although a lot depends on the physics and animation of the cannon impacts.
	// UClass* Telescope;


	
};
