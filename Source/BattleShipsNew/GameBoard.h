// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBoard.generated.h"


UCLASS()
class BATTLESHIPSNEW_API AGameBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// TODO: Think of a way to connect this class with the gameboards we have inside of the game. 
	// This would make it easier to lock on with targeting using the static mesh transform position of the actors.

	
};
