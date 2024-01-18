// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectDMSGame : ModuleRules
{
	public ProjectDMSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"ProjectDMSGame"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {
            }
		);

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
			"InputCore",
            "GameplayTags",
            "ModularGameplay",
			"GameFeatures",
			"Niagara",
            "DMSCore",
			"OnlineSubsystem", 
			"OnlineSubsystemNull", 
			"OnlineSubsystemSteam", 
			"OnlineSubsystemUtils"
			
        });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Steamworks" });

		AddEngineThirdPartyPrivateStaticDependencies(Target, "Steamworks");
		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
