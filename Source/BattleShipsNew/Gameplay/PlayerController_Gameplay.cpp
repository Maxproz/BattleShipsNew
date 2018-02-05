// @Maxpro 2018

#include "PlayerController_Gameplay.h"

#include "BattleShipsGameInstance.h"

#include "Runtime/Engine/Classes/Engine/World.h"

void APlayerController_Gameplay::HandleReturnToMainMenu()
{
	// TODO: Do this scoreboard stuff later ~ this will probably be map/board related hide UI code
	// OnHideScoreboard();
	CleanupSessionOnReturnToMenu();

}


/** Ends and/or destroys game session */
void APlayerController_Gameplay::CleanupSessionOnReturnToMenu()
{
	UBattleShipsGameInstance * FGI = GetWorld() != NULL ? Cast<UBattleShipsGameInstance>(GetWorld()->GetGameInstance()) : NULL;

	if (ensure(FGI != NULL))
	{
		// TODO: IMPORTANT: This appears to be an important spot where the game instance gets notified 
		// FGI->CleanupSessionOnReturnToMenu();
	}
}
