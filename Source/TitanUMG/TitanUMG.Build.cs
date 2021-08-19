// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TitanUMG : ModuleRules
{
	public TitanUMG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore","ApplicationCore","RenderCore",
			"RHI","Projects" });

			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		/*PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Slate",
				"SlateCore",
				"RenderCore",
				"RHI",
			}
		);*/
		
		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"ImageWrapper",
				"TargetPlatform",
			}
		);
		if (Target.Type != TargetType.Server)
		{
			PrivateIncludePathModuleNames.AddRange(
				new string[] {
					"SlateRHIRenderer",
				}
			);

			DynamicallyLoadedModuleNames.AddRange(
				new string[] {
					"ImageWrapper",
					"SlateRHIRenderer",
				}
			);
		}
		
	}
}