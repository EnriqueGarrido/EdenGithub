// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EdenValley : ModuleRules
{
	public EdenValley(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "EdenValleyLoadingScreen",
                "Slate",
                "SlateCore",
                "InputCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks"
            }
        );
    }
}
