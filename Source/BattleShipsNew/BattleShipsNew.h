// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


// TODO: Handle the notes/problems inside of the Gameplay/GameMode class 
// TODO: Somehow find/create a mini map class of a battleship gameboard that the players commander will be holding.
// TODO: Do I need a class dedicated to playing cutscenes/animations? whats the best way to handle it?
// TODO: Should I handle the AI board setup in the AICommander class or in the gamemode?
// Or put the logic in the gamemode and have the commander call the functions on game start?
// TODO: Because I made the ship subclasses pawns it should be possible to possess the outline of the ship on mouse click at the start of a game, using the gamemode somehow.
// And then place the ship onto the board with another mouseclick which will unpossess that ship.
// Ideally when that pawn outline is placed on the gameboard the bigship should spawn in the arena???!??!?
// TODO: Look at the notes I added in the game instance class and then try to hammer out some of those problems.
