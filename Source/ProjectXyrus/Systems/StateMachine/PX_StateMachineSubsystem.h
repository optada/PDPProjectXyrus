// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_ScenarioListBase.h"
#include "PX_ServiceManager.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "PX_StateMachineSubsystem.generated.h"


USTRUCT(BlueprintType)
struct FStateMachinePresetStruct
{
	GENERATED_BODY()

	UPROPERTY(config, EditAnywhere)
	TSubclassOf<UPX_ScenarioListBase> ScenarioListClass;
	
	UPROPERTY(config, EditAnywhere)
	TMap<FString, TSubclassOf<UPX_StateBase>> StateClasses;
};


// State Machine settings
UCLASS(config=Game, defaultconfig, notplaceable, meta=(DisplayName="State Machine Settings"))
class PROJECTXYRUS_API UPX_StateMachineSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	// List of presets for StateMachine
	UPROPERTY(Config, EditAnywhere)
	TMap<FString, FStateMachinePresetStruct> StateMachinePresets;

	// List of presets for StateMachine
	UPROPERTY(Config, EditAnywhere)
	TMap<FString, TSubclassOf<UPX_StateBase>> ServiceStatesList;
};


UCLASS()
class PROJECTXYRUS_API UPX_StateMachineSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void PrintDebugLog() const; // Will fire debug information
	
private:
	// Will not be cleared after changing preset for StateMachine, should be cleared manually
	UPROPERTY()
	UPX_ServiceManager* ServiceManager;
	
	
	FString PresetKey = "";
	FStateMachinePresetStruct PresetData;
	
	UPROPERTY()
	UPX_ScenarioListBase* ScenarioList = nullptr;
	
	UPROPERTY()
	TArray<UPX_StateBase*> StateList;
	
public:
	UPX_StateMachineSubsystem();
	
	// This will remove all states, but will not
	void ChangeStateMachinePreset(const FString& StateMachinePresetKey);
	
	// Will call a scenario if it exist
	void ActivateScenario(const uint32& ScenarioKey, const IStatePacket* Data = nullptr) const;
	
	// Will create or unfreeze State if it was frozen
	void ActivateState(const FString& StateKey);

	// Will deactivate and destroy State
	void DeactivateState(const FString& StateKey);

	// Will freeze state if it exist
	void FreezeState(const FString& StateKey);

	
	const UPX_ServiceManager& GetServiceManager() const;

private:
	// Will empty data before update preset 
	void EmptyStateMachinePreset();
	
	bool CreateState(const FString& StateKey);
	void DeleteState(const FString& StateKey);

	UPX_StateBase* FindCreatedState(const FString& StateKey);
};
