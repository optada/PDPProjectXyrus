// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_GameModeGameplayTest.h"

#include "OnlineSessionSettings.h"
#include "ProjectXyrus/GameModes/Network/PX_DefaultSessionSubsystem.h"


DECLARE_LOG_CATEGORY_EXTERN(APX_GameModeGameplayTest_LOG, Log, All);
DEFINE_LOG_CATEGORY(APX_GameModeGameplayTest_LOG);


void APX_GameModeGameplayTest::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance == nullptr)
	{
		UE_LOG(APX_GameModeGameplayTest_LOG, Warning, TEXT("APX_GameModeGameplayTest::InitGame | GameInstance is null"));
		return;
	}
	
	UPX_SessionSubsystemBase* MySubsystem = GameInstance->GetSubsystem<UPX_DefaultSessionSubsystem>();
	if (MySubsystem == nullptr)
	{
		UE_LOG(APX_GameModeGameplayTest_LOG, Warning, TEXT("APX_GameModeGameplayTest::InitGame | MySubsystem is null"));
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPrivateConnections = 0;
	SessionSettings.NumPublicConnections = 10;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bShouldAdvertise = true;

	SessionSettings.Set(FName("SESSION_NAME_KEY"), FString("MySessionName"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	MySubsystem->CreateSession(SessionSettings);
}

void APX_GameModeGameplayTest::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void APX_GameModeGameplayTest::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APX_GameModeGameplayTest::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void APX_GameModeGameplayTest::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
