// @Maxpro 2018

#include "MainMenuWidget.h"

#include "BattleShipsNewHUD.h"
#include "Menu/BattleShipsPlayerController_Menu.h"

#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABattleShipsPlayerController_Menu* MPC = Cast<ABattleShipsPlayerController_Menu>(GetOwningPlayer());
	ABattleShipsNewHUD* PlayerHUDRef = MPC->GetPlayerHUD();


	if (!MPC)
	{
		return;
	}

	PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickedPlayButton);
	SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickedSettingsButton);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickedQuitButton);

}



// Make the Widget switch show the menu when back is clicked
void UMainMenuWidget::OnClickedPlayButton()
{
	GetWorld()->ServerTravel(GameMapName);

}

// if the player clicked to find game, show the server menu and remove this widget from the viewport
void UMainMenuWidget::OnClickedSettingsButton()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("There is no function bound to this button")));

}

void UMainMenuWidget::OnClickedQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit);

}