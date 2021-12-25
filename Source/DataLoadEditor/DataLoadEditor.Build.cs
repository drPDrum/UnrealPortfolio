// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DataLoadEditor : ModuleRules
{
	public DataLoadEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		// PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core",
			"Engine",
			"CoreUObject",
			"InputCore",
			"LevelEditor",
			"Slate",
			"EditorStyle",
			"AssetTools",
			"EditorWidgets",
			"UnrealEd",
			"BlueprintGraph",
			"AnimGraph",
			"ComponentVisualizers",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AppFramework",
			"SlateCore",
			"AnimGraph",
			"UnrealEd",
			"KismetWidgets",
			"MainFrame",
			"PropertyEditor",
			"ComponentVisualizers",
			"DataLoadLib",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
