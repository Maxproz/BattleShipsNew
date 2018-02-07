// @Maxpro 2018

#include "BattleShipsGameInstance.h"

#include "BattleShipMasterPlayerController.h"

// Need the Gameplay GameState for the EndPlayingState() function
#include "Gameplay/BattleShipsGameState_Gameplay.h"
#include "Gameplay/PlayerController_Gameplay.h"

#include "Menu/BattleShipsPlayerController_Menu.h"
#include "BattleShipsNewHUD.h"
#include "Widgets/Menu/MainMenuWidget.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/Package.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"
#include "Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Runtime/Core/Public/UObject/WeakObjectPtrTemplates.h"
#include "Runtime/Core/Public/Misc/CoreDelegates.h"


namespace BattleShipsGameInstanceState
{
	const FName None = FName(TEXT("None"));
	const FName MainMenu = FName(TEXT("MainMenu"));
	const FName MessageMenu = FName(TEXT("MessageMenu"));
	const FName Lobby = FName(TEXT("Lobby"));
	const FName Playing = FName(TEXT("Playing"));
}

UBattleShipsGameInstance::UBattleShipsGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsLicensed(true) // Default to licensed (should have been checked by OS on boot)
{
	CurrentState = BattleShipsGameInstanceState::None;
	LanPlayerName = "Player";

}

void UBattleShipsGameInstance::Init()
{
	Super::Init();

	FCoreDelegates::OnSafeFrameChangedEvent.AddUObject(this, &UBattleShipsGameInstance::HandleSafeFrameChanged);

	FCoreUObjectDelegates::PostLoadMap.AddUObject(this, &UBattleShipsGameInstance::OnPostLoadMap);


	// Register delegate for ticker callback
	TickDelegate = FTickerDelegate::CreateUObject(this, &UBattleShipsGameInstance::Tick);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(TickDelegate);
}

void UBattleShipsGameInstance::Shutdown()
{
	Super::Shutdown();

	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}



void UBattleShipsGameInstance::OnPostLoadMap()
{
	//// Make sure we hide the loading screen when the level is done loading
	//UFusionGameViewportClient* FusionViewport = Cast<UFusionGameViewportClient>(GetGameViewportClient());

	//if (FusionViewport != NULL)
	//{
	//	FusionViewport->HideLoadingScreen();
	//}


}

void UBattleShipsGameInstance::StartGameInstance()
{
	GotoInitialState();
}

FName UBattleShipsGameInstance::GetInitialState()
{
	return BattleShipsGameInstanceState::MainMenu;
}

void UBattleShipsGameInstance::GotoInitialState()
{
	GotoState(GetInitialState());
}

//void UBattleShipsGameInstance::ShowMessageThenGotoState(const FText& Message, const FName& NewState, const bool OverrideExisting, TWeakObjectPtr< ULocalPlayer > PlayerOwner)
//{
//
//	UE_LOG(LogCore, Fatal, TEXT("ShowMessageThenGotoState: Message: %s, NewState: %s"), *Message.ToString(), *NewState.ToString());
//
//	const bool bAlreadyAtMessageMenu = PendingState == BattleShipsGameInstanceState::MessageMenu || CurrentState == BattleShipsGameInstanceState::MessageMenu;
//	const bool bAlreadyAtDestState = PendingState == NewState || CurrentState == NewState;
//
//	// If we are already going to the message menu, don't override unless asked to
//	if (bAlreadyAtMessageMenu && PendingMessage.NextState == NewState && !OverrideExisting)
//	{
//		UE_LOG(LogCore, Fatal, TEXT("ShowMessageThenGotoState: Ignoring due to higher message priority in queue (check 1)."));
//		return;
//	}
//
//	// If we are already at the dest state, don't override unless asked
//	if (bAlreadyAtDestState && !OverrideExisting)
//	{
//		UE_LOG(LogCore, Fatal, TEXT("ShowMessageThenGotoState: Ignoring due to higher message priority in queue (check 3)"));
//		return;
//	}
//
//	PendingMessage.DisplayString = Message;
//	PendingMessage.NextState = NewState;
//	PendingMessage.PlayerOwner = PlayerOwner;
//
//	//if (CurrentState == BattleShipsGameInstanceState::MessageMenu)
//	//{
//	//	UE_LOG(LogCore, Fatal, TEXT("ShowMessageThenGotoState: Forcing new message"));
//	//	EndMessageMenuState();
//	//	BeginMessageMenuState();
//	//}
//	//else
//	//{
//	//	GotoState(BattleShipsGameInstanceState::MessageMenu);
//	//}
//}

void UBattleShipsGameInstance::ShowLoadingScreen()
{
	//// This can be confusing, so here is what is happening:
	////	For LoadMap, we use the IShooterGameLoadingScreenModule interface to show the load screen
	////  This is necessary since this is a blocking call, and our viewport loading screen won't get updated.
	////  We can't use IShooterGameLoadingScreenModule for seamless travel though
	////  In this case, we just add a widget to the viewport, and have it update on the main thread
	////  To simplify things, we just do both, and you can't tell, one will cover the other if they both show at the same time

	///* TODO: Need to figure out how this is implemented*/
	//IFusionGameLoadingScreenModule* const LoadingScreenModule = FModuleManager::LoadModulePtr<IFusionGameLoadingScreenModule>("FusionGameLoadingScreen");
	//if (LoadingScreenModule != nullptr)
	//{
	//	LoadingScreenModule->StartInGameLoadingScreen();
	//}

	//UFusionGameViewportClient* FusionViewport = Cast<UFusionGameViewportClient>(GetGameViewportClient());

	//if (FusionViewport != NULL)
	//{
	//	FusionViewport->ShowLoadingScreen();
	//}
}

bool UBattleShipsGameInstance::LoadFrontEndMap(const FString& MapName)
{
	bool bSuccess = true;

	// if already loaded, do nothing
	UWorld* const World = GetWorld();
	if (World)
	{
		FString const CurrentMapName = *World->PersistentLevel->GetOutermost()->GetName();
		//if (MapName.Find(TEXT("Highrise")) != -1)
		if (CurrentMapName == MapName)
		{
			return bSuccess;
		}
	}

	FString Error;
	EBrowseReturnVal::Type BrowseRet = EBrowseReturnVal::Failure;
	FURL URL(
		*FString::Printf(TEXT("%s"), *MapName)
	);

	if (URL.Valid && !HasAnyFlags(RF_ClassDefaultObject)) //CastChecked<UEngine>() will fail if using Default__ShooterGameInstance, so make sure that we're not default
	{
		BrowseRet = GetEngine()->Browse(*WorldContext, URL, Error);

		// Handle failure.
		if (BrowseRet != EBrowseReturnVal::Success)
		{
			UE_LOG(LogLoad, Fatal, TEXT("%s"), *FString::Printf(TEXT("Failed to enter %s: %s. Please check the log for errors."), *MapName, *Error));
			bSuccess = false;
		}
	}
	return bSuccess;
}




void UBattleShipsGameInstance::ShowMessageThenGoMain(const FText& Message)
{
	// ShowMessageThenGotoState(Message, BattleShipsGameInstanceState::MainMenu);
	// TODO: clean up the unneccessary stuff after you think about how you want to implement these messeges
	GotoState(BattleShipsGameInstanceState::MainMenu);
}


void UBattleShipsGameInstance::GotoState(FName NewState)
{
	UE_LOG(LogTemp, Warning, TEXT("GotoState: NewState: %s"), *NewState.ToString());

	PendingState = NewState;
}

void UBattleShipsGameInstance::MaybeChangeState()
{
	if ((PendingState != CurrentState) && (PendingState != BattleShipsGameInstanceState::None))
	{
		FName const OldState = CurrentState;

		// end current state
		EndCurrentState(PendingState);

		// begin new state
		BeginNewState(PendingState, OldState);

		// clear pending change
		PendingState = BattleShipsGameInstanceState::None;
	}
}

void UBattleShipsGameInstance::EndCurrentState(FName NextState)
{
	// per-state custom ending code here
	if (CurrentState == BattleShipsGameInstanceState::MainMenu)
	{
		EndMainMenuState();
	}
	else if (CurrentState == BattleShipsGameInstanceState::Lobby)
	{
		EndLobbyState();
	}
	else if (CurrentState == BattleShipsGameInstanceState::MessageMenu)
	{
		EndMessageMenuState();
	}
	else if (CurrentState == BattleShipsGameInstanceState::Playing)
	{
		EndPlayingState();
	}

	CurrentState = BattleShipsGameInstanceState::None;
}

void UBattleShipsGameInstance::BeginNewState(FName NewState, FName PrevState)
{
	// per-state custom starting code here
	if (NewState == BattleShipsGameInstanceState::MainMenu)
	{
		BeginMainMenuState();
	}
	else if (NewState == BattleShipsGameInstanceState::Lobby)
	{
		BeginLobbyState();
	}
	else if (NewState == BattleShipsGameInstanceState::MessageMenu)
	{
		BeginMessageMenuState();
	}
	else if (NewState == BattleShipsGameInstanceState::Playing)
	{
		BeginPlayingState();
	}

	CurrentState = NewState;
}



void UBattleShipsGameInstance::BeginMainMenuState()
{
	//// Make sure we're not showing the loadscreen
	//UFusionGameViewportClient* FusionViewport = Cast<UFusionGameViewportClient>(GetGameViewportClient());

	//if (FusionViewport != NULL)
	//{
	//	FusionViewport->HideLoadingScreen();
	//}

	//SetIsOnline(false);


	//// Set presence to menu state for the owning player
	//SetPresenceForLocalPlayers(FVariantData(FString(TEXT("OnMenu"))));

	// load startup map
	LoadFrontEndMap(MainMenuMap);

	// player 0 gets to own the UI
	ULocalPlayer* const Player = GetFirstGamePlayer();


	MainMenuWidget = Cast<ABattleShipsPlayerController_Menu>(Player->GetPlayerController(GetWorld()))->GetPlayerHUD()->GetMainMenuWidget();

	if (MainMenuWidget.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Showing The Valid Main Menu Widget")));
		MainMenuWidget.Get()->ShowWidget();
	}


#if !FUSION_CONSOLE_UI
	// The cached unique net ID is usually set on the welcome screen, but there isn't
	// one on PC/Mac, so do it here.
	if (Player != nullptr)
	{
		Player->SetControllerId(0);
		Player->SetCachedUniqueNetId(Player->GetUniqueNetIdFromCachedControllerId());
	}
#endif

	RemoveNetworkFailureHandlers();
}

void UBattleShipsGameInstance::EndMainMenuState()
{

	if (MainMenuWidget.IsValid())
	{
		MainMenuWidget.Get()->HideWidget();
		MainMenuWidget = nullptr;
	}


}

void UBattleShipsGameInstance::BeginMessageMenuState()
{
	// No Messege menu in this Game yet ( if ever )


	//if (PendingMessage.DisplayString.IsEmpty())
	//{
	//	UE_LOG(LogOnlineGame, Warning, TEXT("UFusionGameInstance::BeginMessageMenuState: Display string is empty"));
	//	GotoInitialState();
	//	return;
	//}

	//// Make sure we're not showing the loadscreen
	//UFusionGameViewportClient* FusionViewport = Cast<UFusionGameViewportClient>(GetGameViewportClient());

	//if (FusionViewport != NULL)
	//{
	//	FusionViewport->HideLoadingScreen();
	//}

	//// player 0 gets to own the UI
	//ULocalPlayer* const Player = GetFirstGamePlayer();

	//AFusionPlayerController_Menu* MenuPC = Cast<AFusionPlayerController_Menu>(Player->GetPlayerController(GetWorld()));
	//MessageMenuUI = MenuPC->GetFusionHUD()->GetMessageMenuWidget();

	//if (MessageMenuUI.IsValid())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Showing The Message Menu Widget From Game")));

	//	MessageMenuUI.Get()->DisplayMessage = PendingMessage.DisplayString;
	//	MessageMenuUI.Get()->OnRep_DisplayMessage();
	//	MessageMenuUI.Get()->ShowWidget();
	//}

	//PendingMessage.DisplayString = FText::GetEmpty();
}

void UBattleShipsGameInstance::EndMessageMenuState()
{
	// No Messege menu in this Game yet ( if ever )


	//if (MessageMenuUI.IsValid())
	//{
	//	MessageMenuUI.Get()->HideWidget();
	//	MessageMenuUI = nullptr;
	//}
}

void UBattleShipsGameInstance::BeginLobbyState()
{
	// No Lobby in this Game yet ( if ever )


	//// Set presence for being in the lobby
	//SetPresenceForLocalPlayers(FVariantData(FString(TEXT("Lobby"))));


	//ULocalPlayer* const Player = GetFirstGamePlayer();
	//AFusionPlayerController_Lobby* LPC = Cast<AFusionPlayerController_Lobby>(Player->GetPlayerController(GetWorld()));

	//if (LPC)
	//{
	//	AFusionHUD* FHUD = Cast<AFusionHUD>(LPC->GetFusionHUD());
	//	if (FHUD)
	//	{

	//		LobbyWidget = FHUD->GetLobbyMenuWidget();
	//		{
	//			if (LobbyWidget.IsValid())
	//			{
	//				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Showing The Valid Lobby Widget")));
	//				LobbyWidget.Get()->ShowWidget();
	//			}
	//		}


	//		/*
	//		LobbyWidget = FHUD->GetLobbyMenuWidget()->TakeWidget();
	//		{
	//		if (LobbyWidget.IsValid())
	//		{
	//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Showing The Valid Lobby Widget")));
	//		LobbyWidget.Get()->SetVisibility(EVisibility::Visible);
	//		}
	//		}
	//		*/

	//	}
	//}
}

void UBattleShipsGameInstance::EndLobbyState()
{
	// No Lobby in this Game yet ( if ever )


	//if (LobbyWidget.IsValid())
	//{
	//	LobbyWidget.Get()->HideWidget();
	//	//LobbyWidget.Get()->SetVisibility(EVisibility::Hidden);
	//	LobbyWidget = nullptr;
	//}


}

void UBattleShipsGameInstance::BeginPlayingState()
{
	// TODO: Nothing else needed here? Seems strange

	// Set presence for playing in a map
	// Variant data is an OnlineSubsystem thing. (not using at the moment.
	// SetPresenceForLocalPlayers(FVariantData(FString(TEXT("InGame"))));

	// Make sure viewport has focus
	// FSlateApplication::Get().SetAllUserFocusToGameViewport();
	// TODO: Figure out if I need this later on...
	// UPDATE: I probably don't need this since im not using slate. 
	// I should be able to control all gameport focus perfectly as long as I don't make any UMG/Logic mistakes in my HUD/PlayerController classes.

}

void UBattleShipsGameInstance::EndPlayingState()
{
	// Clear the players' presence information
	// Variant data is for OnlineSubsystem, not needed
	// SetPresenceForLocalPlayers(FVariantData(FString(TEXT("OnMenu"))));

	UWorld* const World = GetWorld();
	ABattleShipsGameState_Gameplay* const GamePlayGameState = World != NULL ? World->GetGameState<ABattleShipsGameState_Gameplay>() : NULL;

	if (GamePlayGameState)
	{
		// Send round end events for local players
		for (int i = 0; i < LocalPlayers.Num(); ++i)
		{
			auto BattleShipsPC = Cast<APlayerController_Gameplay>(LocalPlayers[i]->PlayerController);
			if (BattleShipsPC)
			{
				// Assuming you can't win if you quit early
				// This function preformed achivement and data tracking info using playerstate, online session interface etc..
				// TODO: I can probably just make a new function in the PlayerController and track it differently and easier.
				// BattleShipsPC->ClientSendRoundEndEvent(false, GamePlayGameState->ElapsedTime);


			}
		}

		// Give the game state a chance to cleanup first
		// This will either 
		// 1. If called from server, will bump clients back to main menu
		// 2. If called on a client it will hide widgets and preform other various cleanup functions
		GamePlayGameState->RequestFinishAndExitToMainMenu();
	}
	else
	{
		// If there is no game state, make sure the session is in a good state
		// Not needed not using sessions
		// CleanupSessionOnReturnToMenu();
	}
}




void UBattleShipsGameInstance::RemoveNetworkFailureHandlers()
{
	// Remove the local session/travel failure bindings if they exist
	if (GEngine->OnTravelFailure().IsBoundToObject(this) == true)
	{
		GEngine->OnTravelFailure().Remove(TravelLocalSessionFailureDelegateHandle);
	}
}

void UBattleShipsGameInstance::AddNetworkFailureHandlers()
{
	// Add network/travel error handlers (if they are not already there)
	if (GEngine->OnTravelFailure().IsBoundToObject(this) == false)
	{
		TravelLocalSessionFailureDelegateHandle = GEngine->OnTravelFailure().AddUObject(this, &UBattleShipsGameInstance::TravelLocalSessionFailure);
	}
}



bool UBattleShipsGameInstance::Tick(float DeltaSeconds)
{
	// Dedicated server doesn't need to worry about game state
	if (IsRunningDedicatedServer() == true)
	{
		return true;
	}


	MaybeChangeState();

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("CurrentState: %s"), *CurrentState.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("PendingState: %s"), *PendingState.ToString()));


	// UFusionGameViewportClient* FusionViewport = Cast<UFusionGameViewportClient>(GetGameViewportClient());

	if (CurrentState != BattleShipsGameInstanceState::MainMenu)
	{
		// If at any point we aren't licensed (but we are after MainMenu) bounce them back to the welcome screen
		if (!bIsLicensed && CurrentState != BattleShipsGameInstanceState::None )// !FusionViewport->IsShowingDialog())
		{
			const FText ReturnReason = NSLOCTEXT("ProfileMessages", "NeedLicense", "The signed in users do not have a license for this game. Please purchase Fusion from the Xbox Marketplace or sign in a user with a valid license.");
			const FText OKButton = NSLOCTEXT("DialogButtons", "OKAY", "OK");

			// ShowMessageThenGotoState(ReturnReason, BattleShipsGameInstanceState::MainMenu);
			// TODO: Need a messege menu functionality if im not going to use the old one.

		}


		//TScriptDelegate<FWeakObjectPtr> OnControllerReconnectDelegate;
		//OnControllerReconnectDelegate.BindUFunction(this, FName("HideDialogMenuTestFunc")); // OnControllerReconnectConfirm

		//TScriptDelegate<FWeakObjectPtr> EmptyFunctionDelegate;
		//EmptyFunctionDelegate.BindUFunction(this, FName("EmptyFunction"));

		//// Show controller disconnected dialog if any local players have an invalid controller
		//if (FusionViewport != NULL &&
		//	!FusionViewport->IsShowingDialog())
		//{
		//	for (int i = 0; i < LocalPlayers.Num(); ++i)
		//	{
		//		if (LocalPlayers[i] && LocalPlayers[i]->GetControllerId() == -1)
		//		{
		//			FusionViewport->ShowDialog(
		//				LocalPlayers[i],
		//				//FText::Format(NSLOCTEXT("ProfileMessages", "PlayerReconnectControllerFmt", "Player {0}, please reconnect your controller."), FText::AsNumber(i + 1)),
		//				FText::FromString(TEXT("please reconnect your controller.")),
		//				EFusionDialogType::ControllerDisconnected,
		//				OnControllerReconnectDelegate,
		//				EmptyFunctionDelegate
		//			);
		//		}
		//	}
		//}
	}

	return true;
}


	
bool UBattleShipsGameInstance::HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld)
{
	bool const bOpenSuccessful = Super::HandleOpenCommand(Cmd, Ar, InWorld);
	if (bOpenSuccessful)
	{
		GotoState(BattleShipsGameInstanceState::Playing);
	}

	return bOpenSuccessful;
}

void UBattleShipsGameInstance::HandleSafeFrameChanged()
{
	UCanvas::UpdateAllCanvasSafeZoneData();
}




FString UBattleShipsGameInstance::GetPlayerName() const
{
	return LanPlayerName;
}



void UBattleShipsGameInstance::TravelLocalSessionFailure(UWorld *World, ETravelFailure::Type FailureType, const FString& ReasonString)
{
	ABattleShipMasterPlayerController* const MasterPC = Cast<ABattleShipMasterPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// TODO: add lobby test here also
	if (MasterPC != nullptr)
	{
		FText ReturnReason = NSLOCTEXT("NetworkErrors", "JoinSessionFailed", "Join Session failed.");
		if (ReasonString.IsEmpty() == false)
		{
			ReturnReason = FText::Format(NSLOCTEXT("NetworkErrors", "JoinSessionFailedReasonFmt", "Join Session failed. {0}"), FText::FromString(ReasonString));
		}

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("NetworkErrors JoinSessionFailed  Join Session failed.")));
		ShowMessageThenGoMain(ReturnReason);
	}
}