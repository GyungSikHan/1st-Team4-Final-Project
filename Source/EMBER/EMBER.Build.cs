// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EMBER : ModuleRules
{
	public EMBER(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        SharedPCHHeaderFile = "Public/EMBER.h";

        PublicDependencyModuleNames.AddRange(
					new string[] 
					{
						"Core", 
						"CoreUObject",
						"Engine",
						"InputCore",
						"EnhancedInput",
						"UMG",
						"Niagara",
						"NetCore",
						"GameplayMessageRuntime",
						"GameplayTags",
						"GameplayTasks",
						"GameplayAbilities",
						"ModularGameplay",
						"CommonUI",
						"CommonInput",
						"CommonGame",
						"AnimGraphRuntime",
						"NavigationSystem",
						"FieldSystemEngine",
						"GeometryCollectionEngine",
                        "AIModule",
                        "Slate",
                        "SlateCore",
                        });
        PublicIncludePaths.AddRange(
                    new string[]
                    {
						ModuleDirectory, // 기본 모듈 디렉토리
						// 새 폴더 추가
						System.IO.Path.Combine(ModuleDirectory, "GameInfo"), 
						System.IO.Path.Combine(ModuleDirectory, "AI"),
						System.IO.Path.Combine(ModuleDirectory, "Component"),
						System.IO.Path.Combine(ModuleDirectory, "Player"),
                        System.IO.Path.Combine(ModuleDirectory, "Item")                        
                    }
                );
        PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime" });

       
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
	
}
