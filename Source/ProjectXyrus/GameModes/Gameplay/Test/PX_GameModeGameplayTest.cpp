// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_GameModeGameplayTest.h"


void APX_GameModeGameplayTest::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
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
