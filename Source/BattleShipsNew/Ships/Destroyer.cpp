// @Maxpro 2018

#include "Destroyer.h"



// Called when the game starts or when spawned
void ADestroyer::BeginPlay()
{
	Super::BeginPlay();

	SetShipSize(2);
	SetShipName(FString("Destroyer"));


}
