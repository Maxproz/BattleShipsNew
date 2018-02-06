// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Runtime/Core/Public/Containers/Ticker.h"

#include "BattleShipsGameInstance.generated.h"




namespace BattleShipsGameInstanceState
{
	extern const FName None;
	extern const FName MainMenu;
	extern const FName MessageMenu;
	extern const FName Lobby;
	extern const FName Playing;
}


/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API UBattleShipsGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
		// Even though its not necessary for such a small scale game that doesnt even have many states or use sessions for multiplayer yet.
		// I like the idea of tracking and handling game-state stuff in this persistent game instance
		// and I think its worth getting practice using it here again in this game to get better at interweaving the States together 
		// between player controllers/gamestate/gamemode/HUD classes
		
		// Hopefully it should be less confusing than shooter games since this is technically single player.
		// At first I should just create the Tracking State variables, and set the states correctly without doing any control logic. 
		// Then once everything appears to be working correctly I might see possible improvements with the many options I have opened up by using this class.
		
		// As of right now I will create the variables only
		// TODO: Look at Fusion and how I handled the state changes then make the necessary modifications.
		// TODO: I know I set the initial state at one of the override functions, I should look at that first.
		// TODO: Afterwards i'm pretty sure I got the gameinstance from the player controllers when the game mode swapped after I loaded levels and then changed the other states from there. ("starting" states)
		// TODO: I think I changed the "leaveing" states by triggers that happened when games ended in the gamestate/gamemode?? or player controller I would need to double check

public:
	UBattleShipsGameInstance(const FObjectInitializer& ObjectInitializer);

	bool Tick(float DeltaSeconds);


	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;


	/** Sends the game to the specified state. */
	void GotoState(FName NewState);

	/** Obtains the initial welcome state, which can be different based on platform */
	FName GetInitialState();

	/** Sends the game to the initial startup/frontend state  */
	void GotoInitialState();



	/**
	* Creates the message menu, clears other menus and sets the KingState to Message.
	*
	* @param	Message				Main message body
	* @param	OKButtonString		String to use for 'OK' button
	* @param	CancelButtonString	String to use for 'Cancel' button
	* @param	NewState			Final state to go to when message is discarded
	*/
	// NOTE: Too confusing to use this function without the old message menu functioanlity.
	// void ShowMessageThenGotoState(const FText& Message, const FName& NewState, const bool OverrideExisting = true, TWeakObjectPtr< ULocalPlayer > PlayerOwner = nullptr);



	// TSharedPtr< const FUniqueNetId > GetUniqueNetIdFromControllerId(const int ControllerId);



	// Generic confirmation handling (just hide the dialog)
	bool HasLicense() const { return bIsLicensed; }



	/**
	* called from the player state to get the player name
	* @retrun		returns empty string if the player is on steam and retruns the LanPlayerName if he is on Lan
	*/
	FString GetPlayerName() const;


	//Lan player name to not use the Computer Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lan")
	FString LanPlayerName;


public:
	FName GetCurrentState() const { return CurrentState; }

	FText LastErrorMessage;

private:

	UPROPERTY(config)
	FString WelcomeScreenMap;

	UPROPERTY(config)
	FString MainMenuMap;

	UPROPERTY()
	FName MainMenuMapp = "/Game/Tracked/Levels/FirstPersonExampleMap";


	FName CurrentState;

	FName PendingState;

	/** URL to travel to after pending network operations */
	FString TravelURL;


	/** Whether the user has an active license to play the game */
	bool bIsLicensed;

	///** Main menu UI */
	TWeakObjectPtr<class UMainMenuWidget> MainMenuWidget;

	///** Message menu (Shown in the even of errors - unable to connect etc) */
	//TWeakObjectPtr<class UFusionMessageMenu_Widget> MessageMenuUI;

	////TSharedPtr<class SWidget> LobbyWidget;
	//TWeakObjectPtr<class ULobbyMenu_Widget> LobbyWidget;




	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;

	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;
	FDelegateHandle TravelLocalSessionFailureDelegateHandle;


	void OnPostLoadMap();


	void MaybeChangeState();
	void EndCurrentState(FName NextState);
	void BeginNewState(FName NewState, FName PrevState);


	void BeginMainMenuState();
	void BeginMessageMenuState();
	void BeginLobbyState();
	void BeginPlayingState();

	void EndMainMenuState();
	void EndMessageMenuState();
	void EndLobbyState();
	void EndPlayingState();

	void ShowLoadingScreen();
	void AddNetworkFailureHandlers();
	void RemoveNetworkFailureHandlers();

	/** Called when there is an error trying to travel to a local session */
	void TravelLocalSessionFailure(UWorld *World, ETravelFailure::Type FailureType, const FString& ErrorString);


	/**
	* Creates the message menu, clears other menus and sets the KingState to Message.
	*
	* @param	Message				Main message body
	*/
	void ShowMessageThenGoMain(const FText& Message);


	bool LoadFrontEndMap(const FString& MapName);


	// Callback to handle safe frame size changes.
	void HandleSafeFrameChanged();

protected:
	bool HandleOpenCommand(const TCHAR* Cmd, FOutputDevice& Ar, UWorld* InWorld);

		
};
