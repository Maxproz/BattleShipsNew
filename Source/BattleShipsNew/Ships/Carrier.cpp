// @Maxpro 2018

#include "Carrier.h"


// Called when the game starts or when spawned
void ACarrier::BeginPlay()
{
	Super::BeginPlay();

	SetShipSize(5);
	SetShipName(FString("Carrier"));

}

