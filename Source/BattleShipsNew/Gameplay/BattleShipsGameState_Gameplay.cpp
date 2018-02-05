// @Maxpro 2018

#include "BattleShipsGameState_Gameplay.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameModeBase.h"

#include "BattleShipsGameMode_Gameplay.h"
#include "PlayerController_Gameplay.h"

// No idea why its making me specifically include these... probably missing a UEngine or something important include in my Main Header
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/GameFramework/WorldSettings.h"
#include "Runtime/Engine/Classes/Engine/World.h"

ABattleShipsGameState_Gameplay::ABattleShipsGameState_Gameplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	MatchState = MatchState::EnteringMap;
	PreviousMatchState = MatchState::EnteringMap;


	RemainingTime = 0;
	bTimerPaused = false;
}

void ABattleShipsGameState_Gameplay::DefaultTimer()
{
	if (IsMatchInProgress())
	{
		++ElapsedTime;
		if (GetNetMode() != NM_DedicatedServer)
		{
			OnRep_ElapsedTime();
		}
	}

	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ABattleShipsGameState_Gameplay::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}



void ABattleShipsGameState_Gameplay::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(TimerHandle_DefaultTimer, this, &ABattleShipsGameState_Gameplay::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation() / GetWorldSettings()->DemoPlayTimeDilation, true);
}

void ABattleShipsGameState_Gameplay::HandleMatchIsWaitingToStart()
{
	// We need to handle Notify BeginPlay() for all clients 

	if (Role != ROLE_Authority)
	{
		// Server handles this in AGameMode::HandleMatchIsWaitingToStart
		GetWorldSettings()->NotifyBeginPlay();
	}
}

void ABattleShipsGameState_Gameplay::HandleMatchHasStarted()
{
	if (Role != ROLE_Authority)
	{
		// Server handles this in AGameMode::HandleMatchHasStarted
		GetWorldSettings()->NotifyMatchStarted();
	}
	else
	{
		// Now that match has started, act like the base class and set replicated flag
		bReplicatedHasBegunPlay = true;
	}
}

void ABattleShipsGameState_Gameplay::HandleMatchHasEnded()
{

}

void ABattleShipsGameState_Gameplay::HandleLeavingMap()
{

}

void ABattleShipsGameState_Gameplay::RequestFinishAndExitToMainMenu()
{

	if (AuthorityGameMode)
	{
		// we are server, tell the gamemode
		ABattleShipsGameMode_Gameplay* const GameMode = Cast<ABattleShipsGameMode_Gameplay>(AuthorityGameMode);
		if (GameMode)
		{
			GameMode->RequestFinishAndExitToMainMenu();
		}
	}
	else
	{
		// we are client, handle our own business
		APlayerController_Gameplay* const PrimaryPC = Cast<APlayerController_Gameplay>(GetGameInstance()->GetFirstLocalPlayerController());

		if (PrimaryPC)
		{
			check(PrimaryPC->GetNetMode() == ENetMode::NM_Client);
			PrimaryPC->HandleReturnToMainMenu();
		}
	}

}

bool ABattleShipsGameState_Gameplay::HasMatchStarted() const
{
	if (GetMatchState() == MatchState::EnteringMap || GetMatchState() == MatchState::WaitingToStart)
	{
		return false;
	}

	return true;
}

void ABattleShipsGameState_Gameplay::HandleBeginPlay()
{
	// Overridden to not do anything, the state machine tells world when to start
}

bool ABattleShipsGameState_Gameplay::IsMatchInProgress() const
{
	if (GetMatchState() == MatchState::InProgress)
	{
		return true;
	}

	return false;
}

bool ABattleShipsGameState_Gameplay::HasMatchEnded() const
{
	if (GetMatchState() == MatchState::WaitingPostMatch || GetMatchState() == MatchState::LeavingMap)
	{
		return true;
	}

	return false;
}

void ABattleShipsGameState_Gameplay::SetMatchState(FName NewState)
{
	if (Role == ROLE_Authority)
	{
		UE_LOG(LogGameState, Log, TEXT("Match State Changed from %s to %s"), *MatchState.ToString(), *NewState.ToString());

		MatchState = NewState;

		// Call the onrep to make sure the callbacks happen
		OnRep_MatchState();
	}
}

void ABattleShipsGameState_Gameplay::OnRep_MatchState()
{
	if (MatchState == MatchState::WaitingToStart || PreviousMatchState == MatchState::EnteringMap)
	{
		// Call MatchIsWaiting to start even if you join in progress at a later state
		HandleMatchIsWaitingToStart();
	}

	if (MatchState == MatchState::InProgress)
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

	PreviousMatchState = MatchState;
}

void ABattleShipsGameState_Gameplay::OnRep_ElapsedTime()
{
	// Blank on purpose
}

float ABattleShipsGameState_Gameplay::GetPlayerStartTime(class AController* Controller) const
{
	return ElapsedTime;
}


void ABattleShipsGameState_Gameplay::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABattleShipsGameState_Gameplay, MatchState);
	DOREPLIFETIME_CONDITION(ABattleShipsGameState_Gameplay, ElapsedTime, COND_InitialOnly);
	DOREPLIFETIME(ABattleShipsGameState_Gameplay, RemainingTime);
	DOREPLIFETIME(ABattleShipsGameState_Gameplay, bTimerPaused);
}
