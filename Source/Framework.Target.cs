// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FrameworkTarget : TargetRules
{
	public FrameworkTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[]
		{
			"Framework",
			"RuntimeUtils",
			"DataLoadLib",
		});
	}
}