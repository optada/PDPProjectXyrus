// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_GameSessionBase.h"

#include "GameFramework/GameSession.h"
#include "Misc/CommandLine.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/NetConnection.h"
#include "Net/OnlineEngineInterface.h"
#include "GameFramework/PlayerState.h"


static TAutoConsoleVariable<int32> CVarMaxPlayersOverride( TEXT( "net.MaxPlayersOverride" ), 0, TEXT( "If greater than 0, will override the standard max players count. Useful for testing full servers." ) );

void APX_GameSessionBase::HandleMatchIsWaitingToStart()
{
}

void APX_GameSessionBase::HandleMatchHasStarted()
{
	UWorld* World = GetWorld();
	if (UOnlineEngineInterface::Get()->DoesSessionExist(World, SessionName))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && !PlayerController->IsLocalController())
			{
				PlayerController->ClientStartOnlineSession();
			}
		}

		FOnlineSessionStartComplete CompletionDelegate = FOnlineSessionStartComplete::CreateUObject(this, &APX_GameSessionBase::OnStartSessionComplete);
		UOnlineEngineInterface::Get()->StartSession(World, SessionName, CompletionDelegate);
	}

	if (STATS && !UE_BUILD_SHIPPING)
	{
		if (FParse::Param(FCommandLine::Get(), TEXT("MatchAutoStatCapture")))
		{
			UE_LOG(LogGameSession, Log, TEXT("Match has started - begin automatic stat capture"));
			GEngine->Exec(GetWorld(), TEXT("stat startfile"));
		}
	}
}

void APX_GameSessionBase::OnStartSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogGameSession, Verbose, TEXT("OnStartSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);
}

void APX_GameSessionBase::HandleMatchHasEnded()
{
	if (STATS && !UE_BUILD_SHIPPING)
	{
		if (FParse::Param(FCommandLine::Get(), TEXT("MatchAutoStatCapture")))
		{
			UE_LOG(LogGameSession, Log, TEXT("Match has ended - end automatic stat capture"));
			GEngine->Exec(GetWorld(), TEXT("stat stopfile"));
		}
	}

	UWorld* World = GetWorld();
	if (UOnlineEngineInterface::Get()->DoesSessionExist(World, SessionName))
	{
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && !PlayerController->IsLocalController())
			{
				PlayerController->ClientEndOnlineSession();
			}
		}

		FOnlineSessionStartComplete CompletionDelegate = FOnlineSessionEndComplete::CreateUObject(this, &APX_GameSessionBase::OnEndSessionComplete);
		UOnlineEngineInterface::Get()->EndSession(World, SessionName, CompletionDelegate);
	}
}

void APX_GameSessionBase::OnEndSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	UE_LOG(LogGameSession, Verbose, TEXT("OnEndSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), bWasSuccessful);
}

bool APX_GameSessionBase::HandleStartMatchRequest()
{
	return false;
}

void APX_GameSessionBase::InitOptions( const FString& Options )
{
	UWorld* const World = GetWorld();
	check(World);
	AGameModeBase* const GameMode = World ? World->GetAuthGameMode() : nullptr;

	MaxPlayers = UGameplayStatics::GetIntOption( Options, TEXT("MaxPlayers"), MaxPlayers );
	MaxSpectators = UGameplayStatics::GetIntOption( Options, TEXT("MaxSpectators"), MaxSpectators );
	
	if (GameMode)
	{
		UClass* PlayerStateClass = GameMode->PlayerStateClass;
		APlayerState const* const DefaultPlayerState = (PlayerStateClass ? GetDefault<APlayerState>(PlayerStateClass) : nullptr);
		if (DefaultPlayerState)
		{
			SessionName = DefaultPlayerState->SessionName;
		}
		else
		{
			UE_LOG(LogGameSession, Error, TEXT("Player State class is invalid for game mode: %s!"), *GameMode->GetName());
		}
	}
}

bool APX_GameSessionBase::ProcessAutoLogin()
{
	UWorld* World = GetWorld();

	FOnlineAutoLoginComplete CompletionDelegate = FOnlineAutoLoginComplete::CreateUObject(this, &ThisClass::OnAutoLoginComplete);
	if (UOnlineEngineInterface::Get()->AutoLogin(World, 0, CompletionDelegate))
	{
		// Async login started
		return true;
	}

	// Not waiting for async login
	return false;
}

void APX_GameSessionBase::OnAutoLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& Error)
{
	UWorld* World = GetWorld();
	if (UOnlineEngineInterface::Get()->IsLoggedIn(World, LocalUserNum))
	{
		RegisterServer();
	}
	else
	{
		RegisterServerFailed();
	}
}

void APX_GameSessionBase::RegisterServer()
{
}

void APX_GameSessionBase::RegisterServerFailed()
{
	UE_LOG(LogGameSession, Warning, TEXT("Autologin attempt failed, unable to register server!"));
}

FString APX_GameSessionBase::ApproveLogin(const FString& Options)
{
	UWorld* const World = GetWorld();
	check(World);

	AGameModeBase* const GameMode = World->GetAuthGameMode();
	check(GameMode);

	int32 SpectatorOnly = 0;
	SpectatorOnly = UGameplayStatics::GetIntOption(Options, TEXT("SpectatorOnly"), SpectatorOnly);

	if (AtCapacity(SpectatorOnly == 1))
	{
		return TEXT( "Server full." );
	}

	int32 SplitscreenCount = 0;
	SplitscreenCount = UGameplayStatics::GetIntOption(Options, TEXT("SplitscreenCount"), SplitscreenCount);

	if (SplitscreenCount > MaxSplitscreensPerConnection)
	{
		UE_LOG(LogGameSession, Warning, TEXT("ApproveLogin: A maximum of %i splitscreen players are allowed"), MaxSplitscreensPerConnection);
		return TEXT("Maximum splitscreen players");
	}

	return TEXT("");
}

void APX_GameSessionBase::PostLogin(APlayerController* NewPlayer)
{
}

void APX_GameSessionBase::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	if (NewPlayer != NULL)
	{
		// Set the player's ID.
		check(NewPlayer->PlayerState);
		NewPlayer->PlayerState->SetPlayerId(GetNextPlayerID());
		NewPlayer->PlayerState->SetUniqueId(UniqueId);
		NewPlayer->PlayerState->RegisterPlayerWithSession(bWasFromInvite);
	}
}

void APX_GameSessionBase::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	UWorld* World = GetWorld();
	if (GetNetMode() != NM_Standalone &&
		UniqueId.IsValid())
	{
		// Remove the player from the session
		UOnlineEngineInterface::Get()->UnregisterPlayer(World, InSessionName, UniqueId);
	}
}

void APX_GameSessionBase::UnregisterPlayers(FName InSessionName, const TArray<FUniqueNetIdRepl>& Players)
{
	UWorld * World = GetWorld();
	if (GetNetMode() != NM_Standalone &&
		Players.Num() > 0)
	{
		// Remove the player from the session
		TArray<FUniqueNetIdWrapper> PlayerIdsAsWrappers;
		for (const FUniqueNetIdRepl& PlayerId : Players)
		{
			PlayerIdsAsWrappers.Emplace(PlayerId);
		}
		UOnlineEngineInterface::Get()->UnregisterPlayers(World, InSessionName, PlayerIdsAsWrappers);
	}
}

void APX_GameSessionBase::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	if (GetNetMode() != NM_Standalone &&
		ExitingPlayer != NULL &&
		ExitingPlayer->PlayerState &&
		ExitingPlayer->PlayerState->GetUniqueId().IsValid())
	{
		UnregisterPlayer(ExitingPlayer->PlayerState->SessionName, ExitingPlayer->PlayerState->GetUniqueId());
	}
}

bool APX_GameSessionBase::AtCapacity(bool bSpectator)
{
	if ( GetNetMode() == NM_Standalone )
	{
		return false;
	}

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if ( bSpectator )
	{
		return ( (GameMode->GetNumSpectators() >= MaxSpectators)
		&& ((GetNetMode() != NM_ListenServer) || (GameMode->GetNumPlayers() > 0)) );
	}
	else
	{
		const int32 MaxPlayersToUse = CVarMaxPlayersOverride.GetValueOnGameThread() > 0 ? CVarMaxPlayersOverride.GetValueOnGameThread() : MaxPlayers;

		return ( (MaxPlayersToUse>0) && (GameMode->GetNumPlayers() >= MaxPlayersToUse) );
	}
}

void APX_GameSessionBase::NotifyLogout(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	// Unregister the player from the online layer
	UnregisterPlayer(InSessionName, UniqueId);
}

void APX_GameSessionBase::NotifyLogout(const APlayerController* PC)
{
	// Unregister the player from the online layer
	UnregisterPlayer(PC);
}

void APX_GameSessionBase::AddAdmin(APlayerController* AdminPlayer)
{
}

void APX_GameSessionBase::RemoveAdmin(APlayerController* AdminPlayer)
{
}

bool APX_GameSessionBase::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
{
	// Do not kick logged admins
	if (KickedPlayer != NULL && Cast<UNetConnection>(KickedPlayer->Player) != NULL)
	{
		if (KickedPlayer->GetPawn() != NULL)
		{
			KickedPlayer->GetPawn()->Destroy();
		}

		KickedPlayer->ClientWasKicked(KickReason);

		if (KickedPlayer != NULL)
		{
			KickedPlayer->Destroy();
		}

		return true;
	}
	return false;
}

bool APX_GameSessionBase::BanPlayer(class APlayerController* BannedPlayer, const FText& BanReason)
{
	return KickPlayer(BannedPlayer, BanReason);
}

void APX_GameSessionBase::ReturnToMainMenuHost()
{
	const FText RemoteReturnReason = NSLOCTEXT("NetworkErrors", "HostHasLeft", "Host has left the game.");
	const FText LocalReturnReason = FText::GetEmpty();

	APlayerController* Controller = NULL;
	FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator();
	for(; Iterator; ++Iterator)
	{
		Controller = Iterator->Get();
		if (Controller && !Controller->IsLocalPlayerController() && Controller->IsPrimaryPlayer())
		{
			// Clients
			Controller->ClientReturnToMainMenuWithTextReason(RemoteReturnReason);
		}
	}

	Iterator.Reset();
	for(; Iterator; ++Iterator)
	{
		Controller = Iterator->Get();
		if (Controller && Controller->IsLocalPlayerController() && Controller->IsPrimaryPlayer())
		{
			Controller->ClientReturnToMainMenuWithTextReason(LocalReturnReason);
			break;
		}
	}
}

void APX_GameSessionBase::PostSeamlessTravel()
{
}

void APX_GameSessionBase::Restart()
{
	Super::Restart();
}

void APX_GameSessionBase::DumpSessionState()
{
	UE_LOG(LogGameSession, Log, TEXT("  MaxPlayers: %i"), MaxPlayers);
	UE_LOG(LogGameSession, Log, TEXT("  MaxSpectators: %i"), MaxSpectators);

	UOnlineEngineInterface::Get()->DumpSessionState(GetWorld());
}

bool APX_GameSessionBase::CanRestartGame()
{
	return true;
}

bool APX_GameSessionBase::GetSessionJoinability(FName InSessionName, FJoinabilitySettings& OutSettings)
{
	UWorld* const World = GetWorld();
	check(World);

	OutSettings.MaxPlayers = MaxPlayers;
	OutSettings.MaxPartySize = MaxPartySize;
	return UOnlineEngineInterface::Get()->GetSessionJoinability(World, InSessionName, OutSettings);
}

void APX_GameSessionBase::UpdateSessionJoinability(FName InSessionName, bool bPublicSearchable, bool bAllowInvites, bool bJoinViaPresence, bool bJoinViaPresenceFriendsOnly)
{
	if (GetNetMode() != NM_Standalone)
	{
		UOnlineEngineInterface::Get()->UpdateSessionJoinability(GetWorld(), InSessionName, bPublicSearchable, bAllowInvites, bJoinViaPresence, bJoinViaPresenceFriendsOnly);
	}
}
