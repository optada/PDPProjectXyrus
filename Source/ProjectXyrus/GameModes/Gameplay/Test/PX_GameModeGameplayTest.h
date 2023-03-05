// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectXyrus/GameModes/BaseClasses/PX_GameModeBase.h"

#include "PX_GameModeGameplayTest.generated.h"


UCLASS()
class PROJECTXYRUS_API APX_GameModeGameplayTest : public APX_GameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
