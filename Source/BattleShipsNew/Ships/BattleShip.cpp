// @Maxpro 2018

#include "BattleShip.h"


// Called when the game starts or when spawned
void ABattleShip::BeginPlay()
{
	Super::BeginPlay();

	SetShipSize(4);
	SetShipName(FString("Battleship"));

}


