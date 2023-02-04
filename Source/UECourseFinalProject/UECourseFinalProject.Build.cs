// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UECourseFinalProject : ModuleRules
{
	public UECourseFinalProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara", "PhysicsCore","NavigationSystem" });

		PublicIncludePaths.AddRange(new string[]
	   {
			"UECourseFinalProject/",
			"UECourseFinalProject/Public/Pickup",
			"UECourseFinalProject/Public/Targets",
			"UECourseFinalProject/Public/Widgets",
			"UECourseFinalProject/Public/Weapon"
	   });
	}
}
