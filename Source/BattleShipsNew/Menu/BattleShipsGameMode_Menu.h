// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleShipsGameMode_Menu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ABattleShipsGameMode_Menu : public AGameModeBase
{
	GENERATED_BODY()


	ABattleShipsGameMode_Menu();

public:

	virtual void RestartPlayer(AController * NewPlayer) override;
	
	
};
