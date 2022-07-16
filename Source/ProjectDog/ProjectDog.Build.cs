// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectDog : ModuleRules
{
	public ProjectDog(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		PrivateIncludePaths.AddRange(new string[] {	"ProjectDog"	});
	}
}
