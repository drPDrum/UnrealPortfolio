// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Framework : ModuleRules
{
	public Framework(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore" ,
			"Niagara",
			
			// Cutscene
			"LevelSequence",
			"MovieScene",
			
			// AI
			"AIModule", 
			"GameplayTasks",
			"NavigationSystem",
			
			// UI
			"UMG", 
			
			// Media
			"MediaAssets",
			
			// Net
			"Networking",
			"Sockets",
			
			// Module
			"DataLoadLib",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// 기본은 주석, UI를 위해 주석 해제
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
