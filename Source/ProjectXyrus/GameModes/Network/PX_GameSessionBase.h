// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "Online/CoreOnline.h"
#include "GameFramework/GameSession.h"

#include "PX_GameSessionBase.generated.h"


class APlayerController;
class Error;
struct FUniqueNetIdRepl;


/**
Acts as a game-specific wrapper around the session interface. The game code makes calls to this when it needs to interact with the session interface.
A game session exists only the server, while running an online game.
*/
UCLASS(config=Game, notplaceable)
class PROJECTXYRUS_API APX_GameSessionBase : public AGameSession
{
	GENERATED_BODY()
	
	// Initialize options based on passed in options string
	virtual void InitOptions(const FString& Options) override;

	//=================================================================================
	// LOGIN
	
	// Allow an online service to process a login if specified on the commandline with -auth_login/-auth_password
	// true if login is in progress | false otherwise
	virtual bool ProcessAutoLogin() override;

    // Delegate triggered on auto login completion
	virtual void OnAutoLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& Error) override;

	// Called from GameMode.PreLogin() and Login().
	virtual FString ApproveLogin(const FString& Options) override;

	// Register a player with the online service session
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;

	// Called by GameMode::PostLogin to give session code chance to do work after PostLogin
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// true if there is no room on the server for an additional player
	virtual bool AtCapacity(bool bSpectator) override;

	//=================================================================================
	// LOGOUT

	// Called when a PlayerController logs out of game.
	virtual void NotifyLogout(const APlayerController* PC) override;
	
	// Called when a player logs out of game.
	virtual void NotifyLogout(FName InSessionName, const FUniqueNetIdRepl& UniqueId) override;
	
	//Unregister a player from the online service session
	virtual void UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId) override;

	// Unregister players from the online service session
	virtual void UnregisterPlayers(FName InSessionName, const TArray< FUniqueNetIdRepl >& Players) override;
	
	// Unregister a player from the online service session
	virtual void UnregisterPlayer(const APlayerController* ExitingPlayer) override;

	// Add a player to the admin list of this session
	virtual void AddAdmin(APlayerController* AdminPlayer) override;

	// Remove a player from the admin list of this session
	virtual void RemoveAdmin(APlayerController* AdminPlayer) override;

	// Forcibly remove player from the server
	// true if player was able to be kicked, false otherwise
	virtual bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason) override;

	// Forcibly remove player from the server and ban them permanently
	// true if player was able to be banned, false otherwise
	virtual bool BanPlayer(APlayerController* BannedPlayer, const FText& BanReason) override;

	// Gracefully tell all clients then local players to return to lobby */
	virtual void ReturnToMainMenuHost() override;

	// called after a seamless level transition has been completed on the *new* GameMode
	// used to reinitialize players already in the game as they won't have *Login() called on them
	virtual void PostSeamlessTravel() override;

	//=================================================================================
	// SESSION INFORMATION

	// Restart the session
	virtual void Restart() override;

	// Allow a dedicated server a chance to register itself with an online service
	virtual void RegisterServer() override;

	// Callback when autologin was expected but failed
	virtual void RegisterServerFailed() override;

	// Get the current joinability settings for a given session
	// true if session exists and data is valid, false otherwise
	virtual bool GetSessionJoinability(FName InSessionName, FJoinabilitySettings& OutSettings) override;

	// Update session join parameters
	virtual void UpdateSessionJoinability(FName InSessionName, bool bPublicSearchable, bool bAllowInvites, bool bJoinViaPresence, bool bJoinViaPresenceFriendsOnly) override;

    // Does the session require push to talk
    // true if a push to talk keybinding is required or if voice is always enabled
	virtual bool RequiresPushToTalk() const override { return bRequiresPushToTalk; }

	// Dump session info to log for debugging
	virtual void DumpSessionState() override;

	//=================================================================================
	// MATCH INTERFACE

	// true if GameSession handled the request, in case it wants to stall for some reason. Otherwise, game mode will start immediately
	virtual bool HandleStartMatchRequest() override;

	// Handle when the match enters waiting to start
	virtual void HandleMatchIsWaitingToStart() override;

	// Handle when the match has started
	virtual void HandleMatchHasStarted() override;

	// Handle when the match has completed
	virtual void HandleMatchHasEnded() override;

	// Called from GameMode.RestartGame()
	virtual bool CanRestartGame() override;

protected:
	virtual void OnStartSessionComplete(FName InSessionName, bool bWasSuccessful) override;
	virtual void OnEndSessionComplete(FName InSessionName, bool bWasSuccessful) override;
};
