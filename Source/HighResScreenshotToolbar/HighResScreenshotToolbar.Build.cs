// Copyright (c) 2026 fairypark. Licensed under the MIT License.

using UnrealBuildTool;

public class HighResScreenshotToolbar : ModuleRules
{
	public HighResScreenshotToolbar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"InputCore",
				"LevelEditor",
				"Projects",
				"Settings",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"UnrealEd"
			}
		);
	}
}
