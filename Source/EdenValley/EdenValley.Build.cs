// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EdenValley : ModuleRules
{
	public EdenValley(ReadOnlyTargetRules Target) 
        : base(Target)
	{
        PrivatePCHHeaderFile = "Public/EdenValley.h";

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
                "MoviePlayer",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks"
            }
        );
    }
}
