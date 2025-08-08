// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class KoreanChess : ModuleRules
{
	public KoreanChess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "UMG", "Paper2D", "LevelSequence","MovieScene" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "KoreanChess/",
            "KoreanChess/Public/",
            "KoreanChess/Public/AnimInstance",
            "KoreanChess/Public/Board",
            "KoreanChess/Public/Character",
            "KoreanChess/Public/Data",
            "KoreanChess/Public/GameInstance",
            "KoreanChess/Public/GameMode",
            "KoreanChess/Public/GameState",
            "KoreanChess/Public/HUD",
            "KoreanChess/Public/PlayerController",
            "KoreanChess/Public/PlayerStart",
            "KoreanChess/Public/Projectile",
            "KoreanChess/Public/Utility",
            "KoreanChess/Public/Widget",
        });
    }
}
