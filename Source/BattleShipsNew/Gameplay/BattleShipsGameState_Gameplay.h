// @Maxpro 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BattleShipsGameState_Gameplay.generated.h"

/**
 * 
 */
UCLASS()
class BATTLESHIPSNEW_API ABattleShipsGameState_Gameplay : public AGameStateBase
{
	GENERATED_BODY()

	ABattleShipsGameState_Gameplay(const FObjectInitializer& ObjectInitializer);


	// Code to deal with the match state machine

	/** Returns the current match state, this is an accessor to protect the state machine flow */
	FName GetMatchState() const { return MatchState; }

	/** Returns true if we're in progress */
	virtual bool IsMatchInProgress() const;

	/** Returns true if match is WaitingPostMatch or later */
	virtual bool HasMatchEnded() const;



protected:


	/** What match state we are currently in */
	UPROPERTY(ReplicatedUsing = OnRep_MatchState, BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName MatchState;

	/** Previous map state, used to handle if multiple transitions happen per frame */
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = GameState)
	FName PreviousMatchState;

	/** Called when the state transitions to WaitingToStart */
	virtual void HandleMatchIsWaitingToStart();

	/** Called when the state transitions to InProgress */
	virtual void HandleMatchHasStarted();

	/** Called when the map transitions to WaitingPostMatch */
	virtual void HandleMatchHasEnded();

	/** Called when the match transitions to LeavingMap */
	virtual void HandleLeavingMap();

public:

	/** Updates the match state and calls the appropriate transition functions, only valid on server */
	void SetMatchState(FName NewState);

	void RequestFinishAndExitToMainMenu();

	/** time left for warmup / match */
	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	/** is timer paused? */
	UPROPERTY(Transient, Replicated)
	bool bTimerPaused;

	/** Elapsed game time since match has started. */
	UPROPERTY(replicatedUsing = OnRep_ElapsedTime, BlueprintReadOnly, Category = GameState)
	int32 ElapsedTime;

	/** Match state has changed */
	UFUNCTION()
	virtual void OnRep_MatchState();

	/** Gives clients the chance to do something when time gets updates */
	UFUNCTION()
	virtual void OnRep_ElapsedTime();

	/** Called periodically, overridden by subclasses */
	virtual void DefaultTimer();

	//~ Begin AActor Interface
	virtual void PostInitializeComponents() override;
	//~ End AActor Interface

	//~ Begin AGameStateBase Interface
	virtual bool HasMatchStarted() const override;
	virtual void HandleBeginPlay() override;
	virtual float GetPlayerStartTime(class AController* Controller) const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End AGameStateBase Interface

protected:

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;


};
