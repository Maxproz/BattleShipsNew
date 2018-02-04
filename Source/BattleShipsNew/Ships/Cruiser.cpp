// @Maxpro 2018

#include "Cruiser.h"


// Called when the game starts or when spawned
void ACruiser::BeginPlay()
{
	Super::BeginPlay();

	SetShipSize(3);
	SetShipName(FString("Cruiser"));


}