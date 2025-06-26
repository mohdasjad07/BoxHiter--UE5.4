// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BoxHiter : ModuleRules
{
	public BoxHiter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","HTTP","Json","JsonUtilities","UMG" });
	}
}
