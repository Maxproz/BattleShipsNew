// @Maxpro 2018

#include "PlayerController_Gameplay.h"

#include "BattleShipsGameInstance.h"
#include "../BattleShipsNewHUD.h"

#include "Runtime/Engine/Classes/Engine/World.h"


void APlayerController_Gameplay::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		GetPlayerHUD()->CreateGameWidgets();
		
		// TODO: I probably dont need this 1 line below anymore?
		// ClientShowInGameHUD();

		UBattleShipsGameInstance* BattleShipsGI = GetWorld() != NULL ? Cast<UBattleShipsGameInstance>(GetWorld()->GetGameInstance()) : NULL;
		
		// TODO: IMPORTANT: Need to figure out the difference between calling playing here, and between calling it 
		// In the game instance on line 588 HandleOpenCommand();
		BattleShipsGI->GotoState(BattleShipsGameInstanceState::Playing);

	}
}


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
		// UPDATE: This is not needed for single player because its only cleaning up a session which we are not using.
		 // FGI->CleanupSessionOnReturnToMenu();

	}
}

ABattleShipsNewHUD * APlayerController_Gameplay::GetPlayerHUD() const
{
	 return Cast<ABattleShipsNewHUD>(GetHUD()); 
}
