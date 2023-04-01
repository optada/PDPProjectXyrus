// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_GameModeMainMenu.h"

#include "OnlineSessionSettings.h"
#include "ProjectXyrus/GameModes/Network/PX_DefaultSessionSubsystem.h"


DECLARE_LOG_CATEGORY_EXTERN(APX_GameModeMainMenu_LOG, Log, All);
DEFINE_LOG_CATEGORY(APX_GameModeMainMenu_LOG);


void APX_GameModeMainMenu::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void APX_GameModeMainMenu::BeginPlay()
{
	Super::BeginPlay();
}

void APX_GameModeMainMenu::FindServers()
{
	UGameInstance* GameInstance = GetGameInstance();
	UPX_SessionSubsystemBase* MySubsystem = GameInstance->GetSubsystem<UPX_DefaultSessionSubsystem>();

	MySubsystem->OnFindSessionsCompleteEvent.RemoveAll(this);
	MySubsystem->OnFindSessionsCompleteEvent.AddUObject(this, &APX_GameModeMainMenu::ServersFounded);
	MySubsystem->OnJoinGameSessionCompleteEvent.RemoveAll(this);
	MySubsystem->OnJoinGameSessionCompleteEvent.AddUObject(this, &APX_GameModeMainMenu::JoinSessionComplete);
	MySubsystem->FindSessions(5, false);
}

void APX_GameModeMainMenu::ServersFounded(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
	FOnlineSessionSearchResult SearchResult;
	for (const auto& Session : SessionResults)
	{
		//Session.Session.SessionInfo.Get();
		UE_LOG(APX_GameModeMainMenu_LOG, Warning, TEXT("APX_GameModeMainMenu::ServersFounded | session in str = %s"), *Session.GetSessionIdStr());

		// we can take session name just like that
		if (FString SessionName; Session.Session.SessionSettings.Get(FName("SESSION_NAME_KEY"), SessionName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Session name is %s"), *SessionName);
		}

		UGameInstance* GameInstance = GetGameInstance();
		UPX_SessionSubsystemBase* MySubsystem = GameInstance->GetSubsystem<UPX_DefaultSessionSubsystem>();

		MySubsystem->JoinGameSession(Session);
		
		return;
	}
}

void APX_GameModeMainMenu::JoinSessionComplete(const EOnJoinSessionCompleteResult::Type JoinResult)
{
	if (JoinResult != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining session failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("JoinSessionComplete, Travel to  session"));
	
	UGameInstance* GameInstance = GetGameInstance();
	UPX_SessionSubsystemBase* MySubsystem = GameInstance->GetSubsystem<UPX_DefaultSessionSubsystem>();
	
	MySubsystem->TryTravelToCurrentSession();
}
