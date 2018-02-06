// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


// TODO: Look at the PlayerController and PlayerState from Fusion (the gameplay ones) and see if there is any code that you think you could use for battleship
// TODO: The main logic of the gameboard is still not implemented. How are you going to do this class?
// TODO: Setup the world settings classes for the menu and gameplay levels. Then test basic functionality, add the OnScreenDebug messages in needed locations.
// TODO: Handle the notes/problems inside of the Gameplay/GameMode class 
// TODO: Get a paper and somehow map the different state machines (Game Instance, GameMode, GameState) and how they are working together (if they even are)
// TODO: Somehow find/create a mini map class of a battleship gameboard that the players commander will be holding.
// TODO: Do I need a class dedicated to playing cutscenes/animations? whats the best way to handle it?
// TODO: Should I handle the AI board setup in the AICommander class or in the gamemode?
// Or put the logic in the gamemode and have the commander call the functions on game start?
// TODO: Because I made the ship subclasses pawns it should be possible to possess the outline of the ship on mouse click at the start of a game, using the gamemode somehow.
// And then place the ship onto the board with another mouseclick which will unpossess that ship.
// Ideally when that pawn outline is placed on the gameboard the bigship should spawn in the arena???!??!?
// TODO: Look at the notes I added in the game instance class and then try to hammer out some of those problems.
// TODO: Important: As I start moving forward in completing the game, I need to polish the gameplay gamemode/gamestate by removing unnecessary code. 
// As of right now its full of a lot of copy pasta multiplayer code that is useless for the battleship game we have in our mind now.



// cut for quick debug code (must have UEngine header in file)
// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("There is no function bound to this button")));