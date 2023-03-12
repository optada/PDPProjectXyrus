// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectXyrus/GameModes/BaseClasses/PX_GameModeBase.h"

#include "PX_GameModeServer.generated.h"


// Contain data for server initialization (Map, parameters, etc...) 
USTRUCT(BlueprintType)
struct FPX_ServerPreset
{
	GENERATED_USTRUCT_BODY()

	// Start server from this map
	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UWorld> StartMap;

	// Will used for StartMap
	UPROPERTY(config, EditAnywhere)
	FString StartMapParameters;
};


// Parameters for server initialization
UCLASS(config=Game, defaultconfig, notplaceable, meta=(DisplayName="Server Startup Settings"))
class PROJECTXYRUS_API UPX_ServerStartupSettings : public UDeveloperSettings {
	GENERATED_BODY()

public:
	// Options key - which map preset we need to run on server
	UPROPERTY(Config, EditAnywhere, Category=ServerPreset, meta=(MetaClass="KeyServerPreset"))
	FString KeyServerPreset;
	
	// List of all server presets which we can call from parameters
	UPROPERTY(Config, EditAnywhere, Category=ServerPreset, meta=(MetaClass="ServerPresetList"))
	TMap<FString, FPX_ServerPreset> ServerPresetList;
	
	// If it is "True" - we will not use "KeyServerPreset" data from parameters, and will use "OverridenServerPreset" data
	UPROPERTY(Config, EditAnywhere, Category=ServerPreset, meta=(MetaClass="UseOverridenServerPreset", DisplayName="Use OverridenServerPreset"))
	bool bUseOverridenServerPreset;
	
	// Overriden "KeyServerPreset" data
	UPROPERTY(Config, EditAnywhere, Category=ServerPreset, meta=(MetaClass="OverridenServerPreset", DisplayName="OverridenServerPreset", Editcondition="bUseOverridenServerPreset"))
	FString OverridenServerPreset;
};


UCLASS(config=Game)
class PROJECTXYRUS_API APX_GameModeServer : public APX_GameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:
	virtual bool InitializeServer(const FString& Options);
};
