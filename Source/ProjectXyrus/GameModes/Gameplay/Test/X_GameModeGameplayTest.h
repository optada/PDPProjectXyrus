// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectXyrus/GameModes/X_GameModeBase.h"
#include "X_GameModeGameplayTest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTXYRUS_API AX_GameModeGameplayTest : public AX_GameModeBase
{
	GENERATED_BODY()

public:

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
