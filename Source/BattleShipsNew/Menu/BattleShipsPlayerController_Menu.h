// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "BattleShipMasterPlayerController.h"
#include "BattleShipsPlayerController_Menu.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ABattleShipsPlayerController_Menu : public ABattleShipMasterPlayerController
{
	GENERATED_BODY()
	
private:

	/** After game is initialized */
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;


public:

	/** Returns a pointer to the game hud. May return NULL. */
	FORCEINLINE class ABattleShipsNewHUD* GetPlayerHUD() const;
	
};
