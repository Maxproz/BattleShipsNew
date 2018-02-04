// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

UCLASS(abstract)
class BATTLESHIPSNEW_API AShip : public APawn
{
	GENERATED_BODY()


private:

	uint32 ShipSize = 0; // Amount of board squares this ship occupies 
	FString ShipName = "";


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Sets default values for this pawn's properties
	AShip();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	FORCEINLINE uint32 GetShipSize() const { return ShipSize; }
	FORCEINLINE FString GetShipName() const { return ShipName; }

	FORCEINLINE void SetShipSize(const uint32& Size) { ShipSize = Size; }
	FORCEINLINE void SetShipName(const FString& Name) { ShipName = Name; }
	
	
};
