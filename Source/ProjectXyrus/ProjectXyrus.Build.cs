// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectXyrus : ModuleRules
{
	public ProjectXyrus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreOnline",
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Networking",
			"GameplayAbilities", 
			"GameplayTags", 
			"GameplayTasks", 
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"NetCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
