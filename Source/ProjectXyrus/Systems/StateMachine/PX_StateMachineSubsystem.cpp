// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_StateMachineSubsystem.h"

#include "PX_StateMachineTest.h"


DECLARE_LOG_CATEGORY_EXTERN(UPX_StateMachineSubsystem_LOG, Log, All);
DEFINE_LOG_CATEGORY(UPX_StateMachineSubsystem_LOG);


void UPX_StateMachineSubsystem::PrintDebugLog() const
{
	UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT(" "));
	UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("---- ---- ---- ---- STATE MACHINE, DEBUG LOG ---- ---- ---- ----"));

	if (StateList.IsEmpty() == true)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("   State machine is empty"));
	}
	else
	{
		for (const auto* State : StateList)
		{
			if (State)
			{
				UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("   State ( %s ) is %s"), *State->GetStateKey(), State->IsFrozen ? *FString("frozen") : *FString("active"));
			}
		}
	}
	
	UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("----------------------------------------------------------------"));
	UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT(" "));
}

UPX_StateMachineSubsystem::UPX_StateMachineSubsystem()
{
	ServiceManager = NewObject<UPX_ServiceManager>(GetTransientPackage(), UPX_ServiceManager::StaticClass());
	checkf(ServiceManager != nullptr, TEXT("UPX_StateMachineSubsystem() | Crytical Error. ServiceManager in StateMachine should be initialized"));
}

void UPX_StateMachineSubsystem::ChangeStateMachinePreset(const FString& StateMachinePresetKey)
{
	EmptyStateMachinePreset();
	
	const UPX_StateMachineSettings* StateMachineSettings = GetDefault<UPX_StateMachineSettings>();
	if (StateMachineSettings == nullptr)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::InitializeStateMachine | Can not get StateMachineSettings, initialization failed for StateMachinePresetKey ( %s )"), *StateMachinePresetKey);
		return;
	}

	const FStateMachinePresetStruct* StateMachinePreset = StateMachineSettings->StateMachinePresets.Find(StateMachinePresetKey);
	if (StateMachinePreset == nullptr)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::InitializeStateMachine | Can not find FStateMachinePresetStruct, initialization failed for StateMachinePresetKey ( %s )"), *StateMachinePresetKey);
		return;
	}
	
	if (IsValid(StateMachinePreset->ScenarioListClass) == false)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::InitializeStateMachine | ScenarioListClass is invalid, initialization failed for StateMachinePresetKey ( %s )"), *StateMachinePresetKey);
		return;
	}

	ScenarioList = NewObject<UPX_ScenarioListTest>(GetTransientPackage(), StateMachinePreset->ScenarioListClass);
	if (ScenarioList == nullptr)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::InitializeStateMachine | NewScenarioList is null, initialization failed for StateMachinePresetKey ( %s )"), *StateMachinePresetKey);
		return;
	}

	// Collect preset data
	PresetKey = StateMachinePresetKey;
	PresetData = *StateMachinePreset;
}

void UPX_StateMachineSubsystem::ActivateScenario(const uint32& ScenarioKey, const IStatePacket* Data) const
{
	if (IsValid(ScenarioList) == false)
	{
		return;
	}

	if (const auto* Scenario = ScenarioList->FindScenario(ScenarioKey))
	{
		Scenario->RunScenario(*this, Data);
	}
}

void UPX_StateMachineSubsystem::ActivateState(const FString& StateKey)
{
	if (UPX_StateBase* State = FindCreatedState(StateKey))
	{
		State->UnfreezeState();
		return;
	}
	
	if (CreateState(StateKey) == false)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::ActivateState | Activation state ( %s ) failed"), *StateKey);
	}
}

void UPX_StateMachineSubsystem::DeactivateState(const FString& StateKey)
{
	DeleteState(StateKey);
}

void UPX_StateMachineSubsystem::FreezeState(const FString& StateKey)
{
	if (UPX_StateBase* State = FindCreatedState(StateKey))
	{
		State->FreezeState();
	}
}

const UPX_ServiceManager& UPX_StateMachineSubsystem::GetServiceManager() const
{
	checkf(ServiceManager != nullptr, TEXT("UPX_StateMachineSubsystem::GetServiceManager | Crytical Error. ServiceManager in StateMachine should be initialized"));
	return *ServiceManager;
}

void UPX_StateMachineSubsystem::EmptyStateMachinePreset()
{
	// destroy ScenarioList
	if (ScenarioList)
	{
		ScenarioList->ConditionalBeginDestroy();
		ScenarioList = nullptr;
	}
	
	// we need to clear old states if it was initialized before
	for (auto* State : StateList)
	{
		if (State)
		{
			State->ConditionalBeginDestroy();
			State = nullptr;
		}
	}
	StateList.Empty();

	// Reset preset information
	PresetKey = "";
	PresetData = FStateMachinePresetStruct();
}

bool UPX_StateMachineSubsystem::CreateState(const FString& StateKey)
{
	// we should not create a new one if we already have a state
	if (FindCreatedState(StateKey))
	{
		return true; // if we already have a state - this is a valid solution for us --> return true
	}
	
	if (PresetKey.IsEmpty())
	{
		return false; // Should not happen - StateMachine is not initialized, JFS
	}
	
	const TSubclassOf<UPX_StateBase>* StateClass = PresetData.StateClasses.Find(StateKey);
	if (StateClass == nullptr || IsValid(*StateClass) == false)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::CreateState | Can not find StateClass for StateKey ( %s )"), *StateKey);
		return false;
	}

	UPX_StateBase* NewState = NewObject<UPX_StateBase>(GetTransientPackage(), *StateClass);
	if (NewState == nullptr)
	{
		UE_LOG(UPX_StateMachineSubsystem_LOG, Error, TEXT("UPX_StateMachineSubsystem::CreateState | Can not create state for StateKey ( %s )"), *StateKey);
		return false;
	}

	NewState->SetStateKey(StateKey);
	StateList.Add(NewState);
		
	return true;
}

void UPX_StateMachineSubsystem::DeleteState(const FString& StateKey)
{
	if (UPX_StateBase* State = FindCreatedState(StateKey))
	{
		// Remove this state from massive
		StateList.Remove(State); 
		
		State->ConditionalBeginDestroy();
		State = nullptr;
	}
}

UPX_StateBase* UPX_StateMachineSubsystem::FindCreatedState(const FString& StateKey)
{
	for (const auto TestedState : StateList)
	{
		if (TestedState && TestedState->GetStateKey() == StateKey)
		{
			return TestedState;
		}
	}

	return nullptr;
}
