// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Ships/Ship.h"
#include "Submarine.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ASubmarine : public AShip
{
	GENERATED_BODY()
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	


};
