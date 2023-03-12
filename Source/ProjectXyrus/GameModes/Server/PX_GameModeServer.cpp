// Copyright Druzhynin Volodymyr (optada), All Rights Reserved.

#include "PX_GameModeServer.h"
#include "Kismet/GameplayStatics.h"


DECLARE_LOG_CATEGORY_EXTERN(APX_GameModeServer_LOG, Log, All);
DEFINE_LOG_CATEGORY(APX_GameModeServer_LOG);


void APX_GameModeServer::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (InitializeServer(Options) == false)
	{
		UE_LOG(APX_GameModeServer_LOG, Error, TEXT("APX_GameModeServer::InitGame | Server initialize failed"));
		return; 
	}
}

bool APX_GameModeServer::InitializeServer(const FString& Options)
{
	// This is a hack: if loading new mep will be failed - we will return on this map
	// So, we need a count-blocker for this case
	static bool InitializationOfServerStarted = false;
	if (InitializationOfServerStarted)
	{
		UE_LOG(APX_GameModeServer_LOG, Error, TEXT("APX_GameModeServer::InitGame | Itialize server called twice (may map loading failed)"));
		return false;
	}
	InitializationOfServerStarted = true; // set this flag on true - and start initialization
	
	UE_LOG(APX_GameModeServer_LOG, Log, TEXT("APX_GameModeServer::InitializeServer | Start initialize server"));
	
	const UPX_ServerStartupSettings* ServerStartupSettings = GetDefault<UPX_ServerStartupSettings>();
	
	// Get server preset
	const FString KeyServerPreset = ServerStartupSettings->bUseOverridenServerPreset
		? ServerStartupSettings->OverridenServerPreset
		: UGameplayStatics::ParseOption(Options, ServerStartupSettings->KeyServerPreset);
	
	const FPX_ServerPreset* ServerPreset = ServerStartupSettings->ServerPresetList.Find(KeyServerPreset);
	if (ServerPreset == nullptr)
	{
		UE_LOG(APX_GameModeServer_LOG, Error, TEXT("APX_GameModeServer::InitializeServer | Can not find ServerPreset"));
		return false;
	}
	
	// Check ServerPreset map
	const FName TargetMapName = FName(*FPackageName::ObjectPathToPackageName(ServerPreset->StartMap.ToString()));
	if (TargetMapName == "")
	{
		UE_LOG(APX_GameModeServer_LOG, Error, TEXT("APX_GameModeServer::InitializeServer | Can not get valid map from ServerPreset"));
		return false;
	}
	
	// Get additional parameters and start TargetMap
	const FString TargetMapParameters = ServerPreset->StartMapParameters;

	// Start map
	UE_LOG(APX_GameModeServer_LOG, Log, TEXT("APX_GameModeServer::InitGame | Run TargetMap ('%s') with parameters ('%s')"), *TargetMapName.ToString(), *TargetMapParameters);
	UGameplayStatics::OpenLevel(this, TargetMapName, true, TargetMapParameters);

	return true;
}
