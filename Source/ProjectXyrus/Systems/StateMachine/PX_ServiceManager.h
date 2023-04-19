// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PX_StateBase.h"

#include "PX_ServiceManager.generated.h"


UCLASS()
class PROJECTXYRUS_API UPX_ServiceManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UPX_StateBase*> ServiceStateList;

public:
	// Will stop and delete all services
	void ClearServiceManager();
	
	// Will create or unfreeze ServiceState if it was frozen
	void ActivateService(const FString& ServiceStateKey);

	// Will deactivate and destroy ServiceState
	void DeactivateService(const FString& ServiceStateKey);

	// Will freeze ServiceState if it exist
	void FreezeService(const FString& ServiceStateKey);
	
private:
	bool CreateServiceState(const FString& ServiceStateKey);
	void DeleteServiceState(const FString& ServiceStateKey);
	
	UPX_StateBase* FindCreatedServiceState(const FString& ServiceStateKey);
};
