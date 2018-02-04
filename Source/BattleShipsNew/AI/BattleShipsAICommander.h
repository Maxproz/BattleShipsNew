// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "BattleShipsNewCharacter.h"
#include "BattleShipsAICommander.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ABattleShipsAICommander : public ABattleShipsNewCharacter
{
	GENERATED_BODY()
	
private:

	
public:

	// This class will handle all the logic for how the enemy commander behaves.
	// He will have to follow the same rules as the player commander.
	// More than likely he will transfer ships based on certain randomized int calulcations
	// depending on how close the player fired to him when the bullet hits the same ship he is on 
	// and it doesnt kill him.
	// Enemy commander will probably never transfer ships unless the player strikes the same ship as him.
	
	

	
};
