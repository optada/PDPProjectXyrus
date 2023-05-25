// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_StateBase.h"

#include "PX_ScenarioListBase.generated.h"


UCLASS()
class PROJECTXYRUS_API UPX_StateMachineScenarioBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void RunScenario(UPX_StateMachineSubsystem& StateMachineSubsystem, const IStatePacket* Data) const {};
};


UCLASS()
class PROJECTXYRUS_API UPX_ScenarioListBase : public UObject
{
	GENERATED_BODY()

protected:
	TMap<uint32, TSubclassOf<UPX_StateMachineScenarioBase>> Scenarios;

public:
	UPX_StateMachineScenarioBase* FindScenario(const uint32& ScenarioKey)
	{
		if (const TSubclassOf<UPX_StateMachineScenarioBase>* ScenarioClass = Scenarios.Find(ScenarioKey))
		{
			return ScenarioClass->GetDefaultObject();
		}
		
		return nullptr;
	};
};
