// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PX_SessionSubsystemBase.h"

#include "PX_DefaultSessionSubsystem.generated.h"


UCLASS()
class PROJECTXYRUS_API UPX_DefaultSessionSubsystem : public UPX_SessionSubsystemBase
{
	GENERATED_BODY()

protected:
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FDelegateHandle UpdateSessionCompleteDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FDelegateHandle StartSessionCompleteDelegateHandle;
	FDelegateHandle EndSessionCompleteDelegateHandle;
	
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
public:
	UPX_DefaultSessionSubsystem();
	
	virtual void CreateSession(FOnlineSessionSettings& NewSessionSettings) override;
	virtual void UpdateSession() override;
	virtual void DestroySession() override;
	
	virtual void StartSession() override;
	virtual void EndSession() override;
	
	virtual void FindSessions(int32 MaxSearchResults, bool IsLANQuery) override;
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SessionResult) override;
	
	virtual bool TryTravelToCurrentSession() override;

protected:
	virtual void OnCreateSessionCompleted(FName SessionName, bool Successful) override;
	virtual void OnUpdateSessionCompleted(FName SessionName, bool Successful) override;
	virtual void OnDestroySessionCompleted(FName SessionName, bool Successful) override;
	
	virtual void OnStartSessionCompleted(FName SessionName, bool Successful) override;
	virtual void OnEndSessionCompleted(FName SessionName, bool Successful) override;
	
	virtual void OnFindSessionsCompleted(bool Successful) override;
	virtual void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result) override;
};
