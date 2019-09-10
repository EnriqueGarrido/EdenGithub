// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class EdenValleyLoadingScreen : ModuleRules
{
	public EdenValleyLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivatePCHHeaderFile = "Public/EdenValleyLoadingScreen.h";

        PCHUsage = PCHUsageMode.UseSharedPCHs;

        PrivateIncludePaths.Add("EdenValleyLoadingScreen/Private");

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "MoviePlayer",
                "Slate",
                "SlateCore",
                "InputCore",
            }
        );
    }
}
