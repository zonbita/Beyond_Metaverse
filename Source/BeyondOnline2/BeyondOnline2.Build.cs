// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BeyondOnline2 : ModuleRules
{
	public BeyondOnline2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HTTP" });

        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Json", "JsonUtilities", "PhysicsCore", "Slate", "SlateCore", "Sockets", "Networking", "ImageWrapper" });
    }
}
