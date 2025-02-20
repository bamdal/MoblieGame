// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MoblieGame : ModuleRules
{
	public MoblieGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"OnlineSubsystem", "OnlineSubsystemUtils","OnlineSubsystemEOS","OnlineSubsystem" , "OnlineSubsystemUtils",
			"HTTP", "Json"
		});
		
		// 모바일용 패키징을 지원하는 경우
		if (Target.Platform == UnrealTargetPlatform.IOS || Target.Platform == UnrealTargetPlatform.Android)
		{
			// 모바일에서 온라인 기능을 사용하려면 이 추가 항목도 필요할 수 있음
			PrivateDependencyModuleNames.Add("OnlineSubsystemNull");
		}
	}
	
	
}

