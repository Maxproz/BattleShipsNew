// @Maxpro 2018

#include "BattleShipsPlayerController_Menu.h"

#include "BattleShipsNewHUD.h"
#include "BattleShipsGameInstance.h"

#include "Runtime/Engine/Classes/Engine/World.h"

void ABattleShipsPlayerController_Menu::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		GetPlayerHUD()->CreateGameWidgets();


		UBattleShipsGameInstance* BShipsGI = GetWorld() != NULL ? Cast<UBattleShipsGameInstance>(GetWorld()->GetGameInstance()) : NULL;
		//FGI->GotoState(FusionGameInstanceState::MainMenu);

		// When the game Instance starts it should already send us to the main menu state and since we are not using the messege menu state 
		// The code below shouldn't be needed

		//if (BShipsGI->GetCurrentState() == FName(TEXT("MessageMenu")))
		//{
		//	//GetFusionHUD()->GetMessageMenuWidget()->ShowWidget();

		//	ULocalPlayer* const Player = GetLocalPlayer();
		//	TWeakObjectPtr<ULocalPlayer> CurrentPlayer = Player;
		//	FGI->ShowMessageThenGotoState(FGI->LastErrorMessage, FusionGameInstanceState::MainMenu, true, CurrentPlayer);
		//}

		bShowMouseCursor = true;
		ClientIgnoreLookInput(true);
		ClientIgnoreMoveInput(true);
	}
}

void ABattleShipsPlayerController_Menu::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

ABattleShipsNewHUD* ABattleShipsPlayerController_Menu::GetPlayerHUD() const
{
	return Cast<ABattleShipsNewHUD>(GetHUD());
}

