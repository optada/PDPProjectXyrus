// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_ScenarioListBase.h"
#include "PX_StateBase.h"

#include "PX_StateMachineTest.generated.h"


UCLASS()
class PROJECTXYRUS_API UPX_StateMachineScenarioTest : public UPX_StateMachineScenarioBase
{
	GENERATED_BODY()

public:
	virtual void RunScenario(const UPX_StateMachineSubsystem& StateMachineSubsystem, const IStatePacket* Data) const override
	{
		//UPX_StateMachineSubsystem* MySubsystem = GameInstance->GetSubsystem<UPX_StateMachineSubsystem>();
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
