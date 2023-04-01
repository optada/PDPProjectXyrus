// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PX_SessionSubsystemBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnUpdateSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnStartSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnEndSessionComplete, bool, Successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnDestroySessionComplete, bool, Successful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FCSOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
DECLARE_MULTICAST_DELEGATE_OneParam(FCSOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);


UCLASS()
class UPX_SessionSubsystemBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;
	FCSOnUpdateSessionComplete OnUpdateSessionCompleteEvent;
	FCSOnDestroySessionComplete OnDestroySessionCompleteEvent;
	
	FCSOnStartSessionComplete OnStartSessionCompleteEvent;
	FCSOnEndSessionComplete OnEndSessionCompleteEvent;

	FCSOnFindSessionsComplete OnFindSessionsCompleteEvent;
	FCSOnJoinSessionComplete OnJoinGameSessionCompleteEvent;

protected:
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	TSharedPtr<FOnlineSessionSearch> SessionSearchResult;
	
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnUpdateSessionCompleteDelegate UpdateSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	
public:
	UPX_SessionSubsystemBase();

	virtual void CreateSession(FOnlineSessionSettings& NewSessionSettings) {}
	virtual void UpdateSession() {}
	virtual void DestroySession() {}
	
	virtual void StartSession() {}
	virtual void EndSession() {}
	
	virtual void FindSessions(int32 MaxSearchResults, bool IsLANQuery) {}
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SessionResult) {}
	
	virtual bool TryTravelToCurrentSession() { return false; }

protected:
	virtual void OnCreateSessionCompleted(FName SessionName, bool Successful) {}
	virtual void OnUpdateSessionCompleted(FName SessionName, bool Successful) {}
	virtual void OnDestroySessionCompleted(FName SessionName, bool Successful) {}
	
	virtual void OnStartSessionCompleted(FName SessionName, bool Successful) {}
	virtual void OnEndSessionCompleted(FName SessionName, bool Successful) {}
	
	virtual void OnFindSessionsCompleted(bool Successful) {}
	virtual void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {}
};
