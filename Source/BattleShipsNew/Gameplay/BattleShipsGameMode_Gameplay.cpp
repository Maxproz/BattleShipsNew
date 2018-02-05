// @Maxpro 2018

#include "BattleShipsGameMode_Gameplay.h"

#include "BattleShipsGameState_Gameplay.h"
#include "PlayerController_Gameplay.h"
#include "PlayerState_Gameplay.h"


// No idea why its making me specifically include these... probably missing a UEngine or something important include in my Main Header
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/GameFramework/WorldSettings.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

namespace MatchState
{
	const FName EnteringMap = FName(TEXT("EnteringMap"));
	const FName WaitingToStart = FName(TEXT("WaitingToStart"));
	const FName InProgress = FName(TEXT("InProgress"));
	const FName WaitingPostMatch = FName(TEXT("WaitingPostMatch"));
	const FName LeavingMap = FName(TEXT("LeavingMap"));
	const FName Aborted = FName(TEXT("Aborted"));
}

ABattleShipsGameMode_Gameplay::ABattleShipsGameMode_Gameplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	// Set the defaults for the major classes that I will use.. 

	// PlayerController / Pawn etc....
	MatchState = MatchState::EnteringMap;
	//EngineMessageClass = UEngineMessage::StaticClass();
	GameStateClass = ABattleShipsGameState_Gameplay::StaticClass();

}


void ABattleShipsGameMode_Gameplay::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ABattleShipsGameMode_Gameplay::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

}

void ABattleShipsGameMode_Gameplay::DefaultTimer()
{
	// don't update timers for Play In Editor mode, it's not real match
	if (GetWorld()->IsPlayInEditor())
	{
		// start match if necessary.
		if (GetMatchState() == MatchState::WaitingToStart)
		{
			StartMatch();
		}
		return;
	}

	// If it is a real match.... get the GameState 
	ABattleShipsGameState_Gameplay* const MyGameState = Cast<ABattleShipsGameState_Gameplay>(GameState);
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused)
	{
		// Inside the game state, if the reminaing time is > 0 and we are not paused subtract 1 from the time.
		MyGameState->RemainingTime--;

		if (MyGameState->RemainingTime <= 0)
		{
			// If the time is <= 0 we need to handle that state inside of the game mode.. here RestartGame()
			if (GetMatchState() == MatchState::WaitingPostMatch)
			{
				RestartGame();
			}
			else if (GetMatchState() == MatchState::InProgress)
			{
				// If <= 0 time and InProgress we need to finish the match and let the Players (one at the moment) know so  can update their HUDS etc...
				FinishMatch();

				// Send end round events
				for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
				{
					APlayerController_Gameplay* PlayerController = Cast<APlayerController_Gameplay>(*It);

					if (PlayerController && MyGameState)
					{
						APlayerState_Gameplay* PlayerState = Cast<APlayerState_Gameplay>((*It)->PlayerState);
						// const bool bIsWinner = IsWinner(PlayerState);

						// TODO: Check this function and find out what I need for just single player at the moment.
						// PlayerController->ClientSendRoundEndEvent(bIsWinner, MyGameState->ElapsedTime);
					}
				}
			}
			else if (GetMatchState() == MatchState::WaitingToStart)
			{
				// If time <= and waiting to Start just start the match
				StartMatch();
			}
		}
	}
}

void ABattleShipsGameMode_Gameplay::FinishMatch()
{
	ABattleShipsGameState_Gameplay* const MyGameState = Cast<ABattleShipsGameState_Gameplay>(GameState);
	if (IsMatchInProgress())
	{
		EndMatch();
		DetermineMatchWinner();

		// notify players
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			APlayerState_Gameplay* PlayerState = Cast<APlayerState_Gameplay>((*It)->PlayerState);
			const bool bIsWinner = IsWinner(PlayerState);

			(*It)->GameHasEnded(NULL, bIsWinner);
		}

		// lock all pawns
		// pawns are not marked as keep for seamless travel, so we will create new pawns on the next match rather than
		// turning these back on.
		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		// set up to restart the match
		// TODO: What should I set this to? Do I need it?
		MyGameState->RemainingTime = 100;
	}
}


void ABattleShipsGameMode_Gameplay::InitGameState()
{
	Super::InitGameState();

	ABattleShipsGameState_Gameplay* const MyGameState = Cast<ABattleShipsGameState_Gameplay>(GameState);
	if (MyGameState)
	{
		// TODO: Not sure what was here before, but I guess it was not very important
	}
}

bool ABattleShipsGameMode_Gameplay::CanDealDamage(APlayerState_Gameplay* DamageInstigator, class APlayerState_Gameplay* DamagedPlayer) const
{
	if (bAllowFriendlyFireDamage)
	{
		return true;
	}

	/* Allow damage to self */
	if (bAllowSelfDamage)
	{
		return true;
	}

	// TODO: Fix
	// Compare Team Colors
	// return DamageInstigator && DamagedPlayer && (DamageInstigator->GetTeamNum() != DamagedPlayer->GetTeamNum());
	return true;
}


void ABattleShipsGameMode_Gameplay::DetermineMatchWinner()
{
	// TODO: Fix

	ABattleShipsGameState_Gameplay const* const MyGameState = Cast<ABattleShipsGameState_Gameplay>(GameState);
	int32 BestScore = MAX_uint32;
	int32 BestTeam = -1;
	int32 NumBestTeams = 1;

	//for (int32 i = 0; i < MyGameState->TeamScores.Num(); i++)
	//{
	//	const int32 TeamScore = MyGameState->TeamScores[i];
	//	if (BestScore < TeamScore)
	//	{
	//		BestScore = TeamScore;
	//		BestTeam = i;
	//		NumBestTeams = 1;
	//	}
	//	else if (BestScore == TeamScore)
	//	{
	//		NumBestTeams++;
	//	}
	//}

	WinnerTeam = (NumBestTeams == 1) ? BestTeam : NumTeams;
}

bool ABattleShipsGameMode_Gameplay::IsWinner(APlayerState_Gameplay* PlayerState) const
{
	// return PlayerState && !PlayerState->IsQuitter() && PlayerState->GetTeamNum() == WinnerTeam;


	// TODO: Fix
	return true;
}

//bool ABattleShipsGameMode_Gameplay::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
//{
//	if (Player)
//	{
//		AFusionPlayerStart* TeamStart = Cast<AFusionPlayerStart>(SpawnPoint);
//		AFusionPlayerState* PlayerState = Cast<AFusionPlayerState>(Player->PlayerState);
//
//		if (PlayerState && TeamStart && TeamStart->SpawnTeam != PlayerState->GetTeamNum())
//		{
//			return false;
//		}
//	}
//
//	return Super::IsSpawnpointAllowed(SpawnPoint, Player);
//}

FString ABattleShipsGameMode_Gameplay::GetDefaultGameClassPath(const FString& MapName, const FString& Options, const FString& Portal) const
{
	// This is called on the CDO
	return GetClass()->GetPathName();
}

TSubclassOf<ABattleShipsGameMode_Gameplay> ABattleShipsGameMode_Gameplay::GetGameModeClass(const FString& MapName, const FString& Options, const FString& Portal) const
{
	// This is called on the CDO
	return GetClass();
}

FString ABattleShipsGameMode_Gameplay::StaticGetFullGameClassName(FString const& Str)
{
	// return UGameMapsSettings::GetGameModeForName(Str);
	// TODO: Fix
	return FString("fix this");
}

FString ABattleShipsGameMode_Gameplay::GetNetworkNumber()
{
	UNetDriver* NetDriver = GetWorld()->GetNetDriver();
	return NetDriver ? NetDriver->LowLevelGetNetworkNumber() : FString(TEXT(""));
}

void ABattleShipsGameMode_Gameplay::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	SetMatchState(MatchState::EnteringMap);

	//// Bind to delegates
	//FGameDelegates::Get().GetMatineeCancelledDelegate().AddUObject(this, &AGameMode::MatineeCancelled);
	//FGameDelegates::Get().GetPendingConnectionLostDelegate().AddUObject(this, &AGameMode::NotifyPendingConnectionLost);
	//FGameDelegates::Get().GetPreCommitMapChangeDelegate().AddUObject(this, &AGameMode::PreCommitMapChange);
	//FGameDelegates::Get().GetPostCommitMapChangeDelegate().AddUObject(this, &AGameMode::PostCommitMapChange);
	//FGameDelegates::Get().GetHandleDisconnectDelegate().AddUObject(this, &AGameMode::HandleDisconnect);
}

void ABattleShipsGameMode_Gameplay::RestartGame()
{
	//if (GameSession->CanRestartGame())
	//{
		if (GetMatchState() == MatchState::LeavingMap)
		{
			return;
		}

		GetWorld()->ServerTravel("?Restart", GetTravelType());
	//}
}

void ABattleShipsGameMode_Gameplay::PostLogin(APlayerController* NewPlayer)
{
	// TODO: This code was in the other PostLogin (Following 2 Lines), its probably not needed 
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	// APlayerState_Gameplay* NewPlayerState = CastChecked<APlayerState_Gameplay>(NewPlayer->PlayerState);

	UWorld* World = GetWorld();

	// TODO: Pretty sure I don't need this stuff? delete?
	// update player count
	if (MustSpectate(NewPlayer))
	{
		// NumSpectators++;
	}
	else if (World->IsInSeamlessTravel() || NewPlayer->HasClientLoadedCurrentWorld())
	{
		// NumPlayers++;
	}
	else
	{
		// NumTravellingPlayers++;
	}

	// save network address for re-associating with reconnecting player, after stripping out port number
	FString Address = NewPlayer->GetPlayerNetworkAddress();
	int32 pos = Address.Find(TEXT(":"), ESearchCase::CaseSensitive);
	NewPlayer->PlayerState->SavedNetworkAddress = (pos > 0) ? Address.Left(pos) : Address;

	// check if this player is reconnecting and already has PlayerState
	// TODO: Do I need this?
	// FindInactivePlayer(NewPlayer);

	Super::PostLogin(NewPlayer);
}

void ABattleShipsGameMode_Gameplay::Logout(AController* Exiting)
{
	APlayerController* PC = Cast<APlayerController>(Exiting);
	if (PC != nullptr)
	{
		// RemovePlayerControllerFromPlayerCount(PC);
		// AddInactivePlayer(PC->PlayerState, PC);
	}

	Super::Logout(Exiting);
}

void ABattleShipsGameMode_Gameplay::StartPlay()
{
	// Don't call super, this class handles begin play/match start itself

	if (MatchState == MatchState::EnteringMap)
	{
		SetMatchState(MatchState::WaitingToStart);
	}

	// Check to see if we should immediately transfer to match start
	if (MatchState == MatchState::WaitingToStart && ReadyToStartMatch())
	{
		StartMatch();
	}
}

void ABattleShipsGameMode_Gameplay::HandleMatchIsWaitingToStart()
{
	//if (GameSession != nullptr)
	//{
	//	GameSession->HandleMatchIsWaitingToStart();
	//}

	// Calls begin play on actors, unless we're about to transition to match start

	if (!ReadyToStartMatch())
	{
		GetWorldSettings()->NotifyBeginPlay();
	}
}

bool ABattleShipsGameMode_Gameplay::ReadyToStartMatch_Implementation()
{
	// If bDelayed Start is set, wait for a manual match start
	if (bDelayedStart)
	{
		return false;
	}

	// By default start when we have > 0 players
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers + NumBots > 0)
		{
			return true;
		}
	}
	return false;
}

void ABattleShipsGameMode_Gameplay::StartMatch()
{
	if (HasMatchStarted())
	{
		// Already started
		return;
	}

	// NOTE: Not multiplayer dont need game session complexity. Remove?
	////Let the game session override the StartMatch function, in case it wants to wait for arbitration
	//if (GameSession->HandleStartMatchRequest())
	//{
	//	return;
	//}

	SetMatchState(MatchState::InProgress);
}

void ABattleShipsGameMode_Gameplay::HandleMatchHasStarted()
{
	// TODO: Do I need all this?
	// GameSession->HandleMatchHasStarted();

	// start human players first
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = *Iterator;
		if ((PlayerController->GetPawn() == nullptr) && PlayerCanRestart(PlayerController))
		{
			RestartPlayer(PlayerController);
		}
	}

	// Make sure level streaming is up to date before triggering NotifyMatchStarted
	GEngine->BlockTillLevelStreamingCompleted(GetWorld());

	// First fire BeginPlay, if we haven't already in waiting to start match
	GetWorldSettings()->NotifyBeginPlay();

	// Then fire off match started
	GetWorldSettings()->NotifyMatchStarted();


	// TODO: This seems like useless code, Remove?
	//// if passed in bug info, send player to right location
	//const FString BugLocString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugLoc"));
	//const FString BugRotString = UGameplayStatics::ParseOption(OptionsString, TEXT("BugRot"));
	//if (!BugLocString.IsEmpty() || !BugRotString.IsEmpty())
	//{
	//	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	//	{
	//		APlayerController* PlayerController = *Iterator;
	//		if (PlayerController->CheatManager != nullptr)
	//		{
	//			PlayerController->CheatManager->BugItGoString(BugLocString, BugRotString);
	//		}
	//	}
	//}

	//if (IsHandlingReplays() && GetGameInstance() != nullptr)
	//{
	//	GetGameInstance()->StartRecordingReplay(GetWorld()->GetMapName(), GetWorld()->GetMapName());
	//}
}

bool ABattleShipsGameMode_Gameplay::ReadyToEndMatch_Implementation()
{
	// By default don't explicitly end match
	return false;
}

void ABattleShipsGameMode_Gameplay::EndMatch()
{
	if (!IsMatchInProgress())
	{
		return;
	}

	SetMatchState(MatchState::WaitingPostMatch);
}

void ABattleShipsGameMode_Gameplay::HandleMatchHasEnded()
{
	// TODO: Don't need code, don't want replays or multiplayer sessions
	 // GameSession->HandleMatchHasEnded();

	//if (IsHandlingReplays() && GetGameInstance() != nullptr)
	//{
	//	GetGameInstance()->StopRecordingReplay();
	//}
}

void ABattleShipsGameMode_Gameplay::StartToLeaveMap()
{
	SetMatchState(MatchState::LeavingMap);
}

void ABattleShipsGameMode_Gameplay::HandleLeavingMap()
{

}

void ABattleShipsGameMode_Gameplay::AbortMatch()
{
	SetMatchState(MatchState::Aborted);
}

void ABattleShipsGameMode_Gameplay::HandleMatchAborted()
{

}

bool ABattleShipsGameMode_Gameplay::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
	{
		return false;
	}

	return true;
}

bool ABattleShipsGameMode_Gameplay::IsMatchInProgress() const
{
	if (GetMatchState() == MatchState::InProgress)
	{
		return true;
	}

	return false;
}

bool ABattleShipsGameMode_Gameplay::HasMatchEnded() const
{
	if (GetMatchState() == MatchState::WaitingPostMatch || GetMatchState() == MatchState::LeavingMap)
	{
		return true;
	}

	return false;
}

void ABattleShipsGameMode_Gameplay::SetMatchState(FName NewState)
{
	if (MatchState == NewState)
	{
		return;
	}

	UE_LOG(LogGameMode, Display, TEXT("Match State Changed from %s to %s"), *MatchState.ToString(), *NewState.ToString());

	MatchState = NewState;

	OnMatchStateSet();

	ABattleShipsGameState_Gameplay* FullGameState = GetGameState<ABattleShipsGameState_Gameplay>();
	if (FullGameState)
	{
		FullGameState->SetMatchState(NewState);
	}

	K2_OnSetMatchState(NewState);
}

void ABattleShipsGameMode_Gameplay::OnMatchStateSet()
{
	// Call change callbacks
	if (MatchState == MatchState::WaitingToStart)
	{
		HandleMatchIsWaitingToStart();
	}
	else if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
	else if (MatchState == MatchState::LeavingMap)
	{
		HandleLeavingMap();
	}
	else if (MatchState == MatchState::Aborted)
	{
		HandleMatchAborted();
	}
}

void ABattleShipsGameMode_Gameplay::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		// Check to see if we should start the match
		if (ReadyToStartMatch())
		{
			UE_LOG(LogGameMode, Log, TEXT("GameMode returned ReadyToStartMatch"));
			StartMatch();
		}
	}
	if (GetMatchState() == MatchState::InProgress)
	{
		// Check to see if we should start the match
		if (ReadyToEndMatch())
		{
			UE_LOG(LogGameMode, Log, TEXT("GameMode returned ReadyToEndMatch"));
			EndMatch();
		}
	}
}

void ABattleShipsGameMode_Gameplay::HandleSeamlessTravelPlayer(AController*& C)
{
	UE_LOG(LogGameMode, Log, TEXT(">> GameMode::HandleSeamlessTravelPlayer: %s "), *C->GetName());

	APlayerController* PC = Cast<APlayerController>(C);
	if (PC != nullptr && PC->GetClass() != PlayerControllerClass)
	{
		if (PC->Player != nullptr)
		{
			// we need to spawn a new PlayerController to replace the old one
			APlayerController* NewPC = SpawnPlayerController(PC->IsLocalPlayerController() ? ROLE_SimulatedProxy : ROLE_AutonomousProxy, PC->GetFocalLocation(), PC->GetControlRotation());
			if (NewPC == nullptr)
			{
				UE_LOG(LogGameMode, Warning, TEXT("Failed to spawn new PlayerController for %s (old class %s)"), *PC->GetHumanReadableName(), *PC->GetClass()->GetName());
				PC->Destroy();
				return;
			}
			else
			{
				PC->SeamlessTravelTo(NewPC);
				NewPC->SeamlessTravelFrom(PC);
				SwapPlayerControllers(PC, NewPC);
				PC = NewPC;
				C = NewPC;
			}
		}
		else
		{
			PC->Destroy();
		}
	}
	else
	{
		// clear out data that was only for the previous game
		C->PlayerState->Reset();
		// create a new PlayerState and copy over info; this is necessary because the old GameMode may have used a different PlayerState class
		APlayerState* OldPlayerState = C->PlayerState;
		C->InitPlayerState();
		OldPlayerState->SeamlessTravelTo(C->PlayerState);
		// we don"t need the old PlayerState anymore
		//@fixme: need a way to replace PlayerStates that doesn't cause incorrect "player left the game"/"player entered the game" messages
		OldPlayerState->Destroy();
	}

	InitSeamlessTravelPlayer(C);

	// Initialize hud and other player details, shared with PostLogin
	GenericPlayerInitialization(C);

	if (PC)
	{
		// This may spawn the player pawn if the game is in progress
		HandleStartingNewPlayer(PC);
	}

	UE_LOG(LogGameMode, Log, TEXT("<< GameMode::HandleSeamlessTravelPlayer: %s"), *C->GetName());
}

void ABattleShipsGameMode_Gameplay::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);

	APlayerController* NewPC = Cast<APlayerController>(NewController);

	if (NewPC != nullptr)
	{
		SetSeamlessTravelViewTarget(NewPC);

		if (!MustSpectate(NewPC))
		{
			NumPlayers++;
			NumTravellingPlayers--;
		}
	}
	else
	{
		NumBots++;
	}
}

void ABattleShipsGameMode_Gameplay::SetSeamlessTravelViewTarget(APlayerController* PC)
{
	PC->SetViewTarget(PC);
}

void ABattleShipsGameMode_Gameplay::PlayerSwitchedToSpectatorOnly(APlayerController* PC)
{
	RemovePlayerControllerFromPlayerCount(PC);
	NumSpectators++;
}

void ABattleShipsGameMode_Gameplay::RemovePlayerControllerFromPlayerCount(APlayerController* PC)
{
	if (PC)
	{
		if (MustSpectate(PC))
		{
			NumSpectators--;
		}
		else
		{
			if (GetWorld()->IsInSeamlessTravel() || PC->HasClientLoadedCurrentWorld())
			{
				NumPlayers--;
			}
			else
			{
				NumTravellingPlayers--;
			}
		}
	}
}

int32 ABattleShipsGameMode_Gameplay::GetNumPlayers()
{
	return NumPlayers + NumTravellingPlayers;
}

int32 ABattleShipsGameMode_Gameplay::GetNumSpectators()
{
	return NumSpectators;
}

void ABattleShipsGameMode_Gameplay::StartNewPlayer(APlayerController* NewPlayer)
{

}

void ABattleShipsGameMode_Gameplay::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// If players should start as spectators, leave them in the spectator state
	if (!bStartPlayersAsSpectators && !MustSpectate(NewPlayer))
	{
		// If match is in progress, start the player
		if (IsMatchInProgress() && PlayerCanRestart(NewPlayer))
		{
			RestartPlayer(NewPlayer);
		}
		// Check to see if we should start right away, avoids a one frame lag in single player games
		else if (GetMatchState() == MatchState::WaitingToStart)
		{
			// Check to see if we should start the match
			if (ReadyToStartMatch())
			{
				StartMatch();
			}
		}
	}
}

bool ABattleShipsGameMode_Gameplay::PlayerCanRestart_Implementation(APlayerController* Player)
{
	if (!IsMatchInProgress())
	{
		return false;
	}

	return Super::PlayerCanRestart_Implementation(Player);
}

void ABattleShipsGameMode_Gameplay::SendPlayer(APlayerController* aPlayer, const FString& FURL)
{
	aPlayer->ClientTravel(FURL, TRAVEL_Relative);
}

bool ABattleShipsGameMode_Gameplay::GetTravelType()
{
	return false;
}

void ABattleShipsGameMode_Gameplay::Say(const FString& Msg)
{
	Broadcast(nullptr, Msg, NAME_None);
}

void ABattleShipsGameMode_Gameplay::Broadcast(AActor* Sender, const FString& Msg, FName Type)
{
	APlayerState* SenderPlayerState = nullptr;
	if (Cast<APawn>(Sender) != nullptr)
	{
		SenderPlayerState = Cast<APawn>(Sender)->PlayerState;
	}
	else if (Cast<AController>(Sender) != nullptr)
	{
		SenderPlayerState = Cast<AController>(Sender)->PlayerState;
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		(*Iterator)->ClientTeamMessage(SenderPlayerState, Msg, Type);
	}
}


void ABattleShipsGameMode_Gameplay::BroadcastLocalized(AActor* Sender, TSubclassOf<ULocalMessage> Message, int32 Switch, APlayerState* RelatedPlayerState_1, APlayerState* RelatedPlayerState_2, UObject* OptionalObject)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		(*Iterator)->ClientReceiveLocalizedMessage(Message, Switch, RelatedPlayerState_1, RelatedPlayerState_2, OptionalObject);
	}
}

void ABattleShipsGameMode_Gameplay::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	check(PlayerState)
		UWorld* LocalWorld = GetWorld();
	// don't store if it's an old PlayerState from the previous level or if it's a spectator... or if we are shutting down
	if (!PlayerState->bFromPreviousLevel && !MustSpectate(PC) && !LocalWorld->bIsTearingDown)
	{
		APlayerState* const NewPlayerState = PlayerState->Duplicate();
		if (NewPlayerState)
		{
			// Side effect of Duplicate() adding PlayerState to PlayerArray (see APlayerState::PostInitializeComponents)
			GameState->RemovePlayerState(NewPlayerState);

			// make PlayerState inactive
			NewPlayerState->SetReplicates(false);

			// delete after some time
			NewPlayerState->SetLifeSpan(InactivePlayerStateLifeSpan);

			// On console, we have to check the unique net id as network address isn't valid
			const bool bIsConsole = GEngine->IsConsoleBuild();
			// Assume valid unique ids means comparison should be via this method
			const bool bHasValidUniqueId = NewPlayerState->UniqueId.IsValid();
			// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
			const bool bHasValidNetworkAddress = !NewPlayerState->SavedNetworkAddress.IsEmpty();
			const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;

			// make sure no duplicates
			for (int32 Idx = 0; Idx < InactivePlayerArray.Num(); ++Idx)
			{
				APlayerState* const CurrentPlayerState = InactivePlayerArray[Idx];
				if ((CurrentPlayerState == nullptr) || CurrentPlayerState->IsPendingKill())
				{
					// already destroyed, just remove it
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
				else if ((!bUseUniqueIdCheck && bHasValidNetworkAddress && (CurrentPlayerState->SavedNetworkAddress == NewPlayerState->SavedNetworkAddress))
					|| (bUseUniqueIdCheck && (CurrentPlayerState->UniqueId == NewPlayerState->UniqueId)))
				{
					// destroy the playerstate, then remove it from the tracking
					CurrentPlayerState->Destroy();
					InactivePlayerArray.RemoveAt(Idx, 1);
					Idx--;
				}
			}
			InactivePlayerArray.Add(NewPlayerState);

			// cap at 16 saved PlayerStates
			if (InactivePlayerArray.Num() > 16)
			{
				int32 const NumToRemove = InactivePlayerArray.Num() - 16;

				// destroy the extra inactive players
				for (int Idx = 0; Idx < NumToRemove; ++Idx)
				{
					APlayerState* const PS = InactivePlayerArray[Idx];
					if (PS != nullptr)
					{
						PS->Destroy();
					}
				}

				// and then remove them from the tracking array
				InactivePlayerArray.RemoveAt(0, NumToRemove);
			}
		}
	}

	PlayerState->OnDeactivated();
}

bool ABattleShipsGameMode_Gameplay::FindInactivePlayer(APlayerController* PC)
{
	check(PC && PC->PlayerState);
	// don't bother for spectators
	if (MustSpectate(PC))
	{
		return false;
	}

	// On console, we have to check the unique net id as network address isn't valid
	const bool bIsConsole = GEngine->IsConsoleBuild();
	// Assume valid unique ids means comparison should be via this method
	const bool bHasValidUniqueId = PC->PlayerState->UniqueId.IsValid();
	// Don't accidentally compare empty network addresses (already issue with two clients on same machine during development)
	const bool bHasValidNetworkAddress = !PC->PlayerState->SavedNetworkAddress.IsEmpty();
	const bool bUseUniqueIdCheck = bIsConsole || bHasValidUniqueId;

	const FString NewNetworkAddress = PC->PlayerState->SavedNetworkAddress;
	const FString NewName = PC->PlayerState->PlayerName;
	for (int32 i = 0; i < InactivePlayerArray.Num(); i++)
	{
		APlayerState* CurrentPlayerState = InactivePlayerArray[i];
		if ((CurrentPlayerState == nullptr) || CurrentPlayerState->IsPendingKill())
		{
			InactivePlayerArray.RemoveAt(i, 1);
			i--;
		}
		else if ((bUseUniqueIdCheck && (CurrentPlayerState->UniqueId == PC->PlayerState->UniqueId)) ||
			(!bUseUniqueIdCheck && bHasValidNetworkAddress && (FCString::Stricmp(*CurrentPlayerState->SavedNetworkAddress, *NewNetworkAddress) == 0) && (FCString::Stricmp(*CurrentPlayerState->PlayerName, *NewName) == 0)))
		{
			// found it!
			APlayerState* OldPlayerState = PC->PlayerState;
			PC->PlayerState = CurrentPlayerState;
			PC->PlayerState->SetOwner(PC);
			PC->PlayerState->SetReplicates(true);
			PC->PlayerState->SetLifeSpan(0.0f);
			OverridePlayerState(PC, OldPlayerState);
			GameState->AddPlayerState(PC->PlayerState);
			InactivePlayerArray.RemoveAt(i, 1);
			OldPlayerState->bIsInactive = true;
			// Set the uniqueId to nullptr so it will not kill the player's registration 
			// in UnregisterPlayerWithSession()
			OldPlayerState->SetUniqueId(nullptr);
			OldPlayerState->Destroy();
			PC->PlayerState->OnReactivated();
			return true;
		}

	}
	return false;
}

void ABattleShipsGameMode_Gameplay::OverridePlayerState(APlayerController* PC, APlayerState* OldPlayerState)
{
	PC->PlayerState->DispatchOverrideWith(OldPlayerState);
}


bool ABattleShipsGameMode_Gameplay::CanServerTravel(const FString& URL, bool bAbsolute)
{
	if (!Super::CanServerTravel(URL, bAbsolute))
	{
		return false;
	}

	// Check for an error in the server's connection
	if (GetMatchState() == MatchState::Aborted)
	{
		UE_LOG(LogGameMode, Log, TEXT("Not traveling because of network error"));
		return false;
	}

	return true;
}

void ABattleShipsGameMode_Gameplay::PostSeamlessTravel()
{

	//if (GameSession)
	//{
	//	GameSession->PostSeamlessTravel();
	//}

	// We have to make a copy of the controller list, since the code after this will destroy
	// and create new controllers in the world's list
	TArray<TAutoWeakObjectPtr<class AController> >	OldControllerList;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		OldControllerList.Add(*It);
	}

	// handle players that are already loaded
	for (FConstControllerIterator Iterator = OldControllerList.CreateConstIterator(); Iterator; ++Iterator)
	{
		AController* Controller = *Iterator;
		if (Controller->PlayerState)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Controller);
			if (PlayerController == nullptr)
			{
				HandleSeamlessTravelPlayer(Controller);
			}
			else
			{
				if (MustSpectate(PlayerController))
				{
					// The spectator count must be incremented here, instead of in HandleSeamlessTravelPlayer,
					// as otherwise spectators can 'hide' from player counters, by making HasClientLoadedCurrentWorld return false
					NumSpectators++;
				}
				else
				{
					NumTravellingPlayers++;
				}
				if (PlayerController->HasClientLoadedCurrentWorld())
				{
					HandleSeamlessTravelPlayer(Controller);
				}
			}
		}
	}
}

bool ABattleShipsGameMode_Gameplay::IsHandlingReplays()
{
	// If we're running in PIE, don't record demos
	if (GetWorld() != nullptr && GetWorld()->IsPlayInEditor())
	{
		return false;
	}

	return bHandleDedicatedServerReplays && GetNetMode() == ENetMode::NM_DedicatedServer;
}

void ABattleShipsGameMode_Gameplay::SetBandwidthLimit(float AsyncIOBandwidthLimit)
{
	// TODO: Probably doesnt matter?
	// GAsyncIOBandwidthLimit = AsyncIOBandwidthLimit;
}

void ABattleShipsGameMode_Gameplay::MatineeCancelled() {}

void ABattleShipsGameMode_Gameplay::PreCommitMapChange(const FString& PreviousMapName, const FString& NextMapName) {}

void ABattleShipsGameMode_Gameplay::PostCommitMapChange() {}

void ABattleShipsGameMode_Gameplay::NotifyPendingConnectionLost() {}

void ABattleShipsGameMode_Gameplay::HandleDisconnect(UWorld* InWorld, UNetDriver* NetDriver)
{
	AbortMatch();
}

bool ABattleShipsGameMode_Gameplay::SpawnPlayerFromSimulate(const FVector& NewLocation, const FRotator& NewRotation)
{
#if WITH_EDITOR
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC != nullptr)
	{
		RemovePlayerControllerFromPlayerCount(PC);
		NumPlayers++;
	}
#endif

	return Super::SpawnPlayerFromSimulate(NewLocation, NewRotation);
}



// Important GameMode/GameState relation code.
void ABattleShipsGameMode_Gameplay::RequestFinishAndExitToMainMenu()
{
	FinishMatch();

	APlayerController_Gameplay* LocalPrimaryController = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController_Gameplay* Controller = Cast<APlayerController_Gameplay>(*Iterator);

		if (Controller == NULL)
		{
			continue;
		}

		if (!Controller->IsLocalController())
		{
			const FString RemoteReturnReason = NSLOCTEXT("NetworkErrors", "HostHasLeft", "Host has left the game.").ToString();
			Controller->ClientReturnToMainMenu(RemoteReturnReason);
		}
		else
		{
			LocalPrimaryController = Controller;
		}
	}

	// GameInstance should be calling this from an EndState.  So call the PC function that performs cleanup, not the one that sets GI state.
	if (LocalPrimaryController != NULL)
	{
		LocalPrimaryController->HandleReturnToMainMenu();
	}
}