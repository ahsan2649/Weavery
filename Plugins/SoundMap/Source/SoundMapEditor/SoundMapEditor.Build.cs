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
                "Core",
                "InputCore",
                "DeveloperSettings",
                "AssetRegistry",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "SoundMap",
                "AssetTools",
                "UnrealEd",
                "AudioWidgets",
                "ToolMenus",
                "EditorStyle",
                "WaveformEditorWidgets",
                "WaveformEditor"
            }
        );
    }
}