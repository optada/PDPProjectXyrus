// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PX_StateBase.generated.h"


class UPX_StateMachineSubsystem;
class UPX_ServiceManager;


struct IStatePacket
{
	IStatePacket() = default;
	IStatePacket(IStatePacket const&) = delete;
	IStatePacket(IStatePacket&&) = delete;
	IStatePacket& operator=(IStatePacket const&) = delete;
	IStatePacket& operator=(IStatePacket&&) = delete;
	virtual ~IStatePacket() = 0;
};


// Base class for States and ServiceStates
// For code-convention all States should named like this: UPX_StateMainMenu
// For code-convention all ServiceStates should named like this: UPX_ServiceStateMainMenu
UCLASS()
class PROJECTXYRUS_API UPX_StateBase : public UObject
{
	GENERATED_BODY()

	// We need this for allow access to SetStateKey(const FString& NewStateKey)
	friend UPX_StateMachineSubsystem;
	friend UPX_ServiceManager;
	
private:
	FString StateKey = "";
	bool IsFrozen = false;

public:
	virtual void OnActivate() {};
	virtual void OnDeactivate() {};

	virtual void OnFreeze() {};
	virtual void OnUnfreeze() {};

	const FString& GetStateKey() const { return StateKey; }
	bool GetIsFrozen() const { return IsFrozen; }
	
private:
	void SetStateKey(const FString& NewStateKey) { StateKey = NewStateKey; }

	void FreezeState()
	{
		if (IsFrozen == false)
		{
			IsFrozen = true;
			OnFreeze();
		}
	}
	
	void UnfreezeState()
	{
		if (IsFrozen)
		{
			IsFrozen = false;
			OnUnfreeze();
		}
	}
};
