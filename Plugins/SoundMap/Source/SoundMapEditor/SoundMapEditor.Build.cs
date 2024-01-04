using UnrealBuildTool;

public class SoundMapEditor : ModuleRules
{
    public SoundMapEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "SoundMap",
                "AssetTools",
                "UnrealEd",
                "AudioWidgets",
            }
        );
    }
}