// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "X_GameModeGameplayTest.h"


void AX_GameModeGameplayTest::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void AX_GameModeGameplayTest::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AX_GameModeGameplayTest::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}
