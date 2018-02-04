// @Maxpro 2018

#include "Submarine.h"


// Called when the game starts or when spawned
void ASubmarine::BeginPlay()
{
	Super::BeginPlay();

	SetShipSize(3);
	SetShipName(FString("Submarine"));

}