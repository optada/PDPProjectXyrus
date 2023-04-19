// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_ServiceManager.h"

#include "PX_StateMachineSubsystem.h"


DECLARE_LOG_CATEGORY_EXTERN(UPX_ServiceManager_LOG, Log, All);
DEFINE_LOG_CATEGORY(UPX_ServiceManager_LOG);


void UPX_ServiceManager::ClearServiceManager()
{
	for (auto* ServiceState : ServiceStateList)
	{
		if (ServiceState)
		{
			ServiceState->ConditionalBeginDestroy();
			ServiceState = nullptr;
		}
	}
	ServiceStateList.Empty();
}

void UPX_ServiceManager::ActivateService(const FString& ServiceStateKey)
{
	if (UPX_StateBase* ServiceState = FindCreatedServiceState(ServiceStateKey))
	{
		ServiceState->UnfreezeState();
		return;
	}
	
	if (CreateServiceState(ServiceStateKey) == false)
	{
		UE_LOG(UPX_ServiceManager_LOG, Error, TEXT("UPX_ServiceManager::ActivateServiceState | Activation ServiceState ( %s ) failed"), *ServiceStateKey);
	}
}

void UPX_ServiceManager::DeactivateService(const FString& ServiceStateKey)
{
	DeleteServiceState(ServiceStateKey);
}

void UPX_ServiceManager::FreezeService(const FString& ServiceStateKey)
{
	if (UPX_StateBase* ServiceState = FindCreatedServiceState(ServiceStateKey))
	{
		ServiceState->FreezeState();
	}
}

bool UPX_ServiceManager::CreateServiceState(const FString& ServiceStateKey)
{
	// we should not create a new one if we already have a state
	if (FindCreatedServiceState(ServiceStateKey))
	{
		return true; // if we already have a state - this is a valid solution for us --> return true
	}

	const UPX_StateMachineSettings* StateMachineSettings = GetDefault<UPX_StateMachineSettings>();
	if (StateMachineSettings == nullptr)
	{
		UE_LOG(UPX_ServiceManager_LOG, Error, TEXT("UPX_ServiceManager::CreateServiceState | Can not get StateMachineSettings, creation ServiceState failed for ServiceStateKey ( %s )"), *ServiceStateKey);
		return false;
	}
	
	const TSubclassOf<UPX_StateBase>* StateClass = StateMachineSettings->ServiceStatesList.Find(ServiceStateKey);
	if (StateClass == nullptr || IsValid(*StateClass) == false)
	{
		UE_LOG(UPX_ServiceManager_LOG, Error, TEXT("UPX_ServiceManager::CreateServiceState | Can not find StateClass for ServiceStateKey ( %s )"), *ServiceStateKey);
		return false;
	}

	UPX_StateBase* NewState = NewObject<UPX_StateBase>(GetTransientPackage(), *StateClass);
	if (NewState == nullptr)
	{
		UE_LOG(UPX_ServiceManager_LOG, Error, TEXT("UPX_ServiceManager::CreateServiceState | Can not create state for ServiceStateKey ( %s )"), *ServiceStateKey);
		return false;
	}

	NewState->SetStateKey(ServiceStateKey);
	ServiceStateList.Add(NewState);
		
	return true;
}

void UPX_ServiceManager::DeleteServiceState(const FString& ServiceStateKey)
{
	if (UPX_StateBase* ServiceState = FindCreatedServiceState(ServiceStateKey))
	{
		// Remove this ServiceState from massive
		ServiceStateList.Remove(ServiceState); 
		
		ServiceState->ConditionalBeginDestroy();
		ServiceState = nullptr;
	}
}

UPX_StateBase* UPX_ServiceManager::FindCreatedServiceState(const FString& ServiceStateKey)
{
	for (const auto TestedServiceState : ServiceStateList)
	{
		if (TestedServiceState && TestedServiceState->GetStateKey() == ServiceStateKey)
		{
			return TestedServiceState;
		}
	}

	return nullptr;
}
