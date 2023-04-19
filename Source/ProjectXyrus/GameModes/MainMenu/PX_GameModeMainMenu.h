// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ProjectXyrus/GameModes/BaseClasses/PX_GameModeBase.h"

#include "PX_GameModeMainMenu.generated.h"


UCLASS()
class PROJECTXYRUS_API APX_GameModeMainMenu : public APX_GameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

protected:
	void ServersFounded(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
	void JoinSessionComplete(const EOnJoinSessionCompleteResult::Type JoinResult);

	UFUNCTION(BlueprintCallable)
	void DebugStateMachine();
	
	UFUNCTION(BlueprintCallable)
	void FindServers();
};
