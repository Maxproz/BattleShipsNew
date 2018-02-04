// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Ships/Ship.h"
#include "Cruiser.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ACruiser : public AShip
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
};
