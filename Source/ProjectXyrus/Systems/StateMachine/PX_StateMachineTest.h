// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_StateMachineSubsystem.h"

#include "PX_StateMachineTest.generated.h"


UCLASS()
class PROJECTXYRUS_API UPX_StateMachineScenarioTest : public UPX_StateMachineScenarioBase
{
	GENERATED_BODY()

public:
	virtual void RunScenario(UPX_StateMachineSubsystem& StateMachineSubsystem, const IStatePacket* Data) const override
	{
		StateMachineSubsystem.ActivateState("");
	};
};


UCLASS()
class PROJECTXYRUS_API UPX_ScenarioListTest : public UPX_ScenarioListBase
{
	GENERATED_BODY()

	UPX_ScenarioListTest()
	{
		Scenarios.Add(0, UPX_StateMachineScenarioTest::StaticClass());
	}
};


UCLASS()
class PROJECTXYRUS_API UPX_StateTest : public UPX_StateBase
{
	GENERATED_BODY()

	virtual void OnActivate() override
	{
		
	};
};
