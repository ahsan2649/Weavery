#include "SoundMapEditorModule.h"

#include "AssetToolsModule.h"
#include "FAssetTypeActions_SoundMap.h"
#include "FSoundMapEditorToolkit.h"
#include "IAssetTools.h"
#include "SoundMapEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSoundMapEditorModule"

void FSoundMapEditorModule::StartupModule()
{
    IModuleInterface::StartupModule();

    FSoundMapEditorCommands::Register();
    
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory("SoundMap Plugin", INVTEXT("SoundMap Plugin"));

    const TSharedRef<IAssetTypeActions> Actions = MakeShared<FAssetTypeActions_SoundMap>(AssetTypeCategory);
    AssetTools.RegisterAssetTypeActions(Actions);
    CreatedAssetTypeActions = Actions;
    
    
}

void FSoundMapEditorModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();
    FSoundMapEditorCommands::Unregister();
    
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions.ToSharedRef());
    }
}

void FSoundMapEditorModule::CreateSoundMapEditor(const EToolkitMode::Type Mode,
    const TSharedPtr<IToolkitHost>& InitToolkitHost, USoundMapAsset* InSoundMapAsset)
{
    TSharedRef<FSoundMapEditorToolkit> SoundMapEditor(new FSoundMapEditorToolkit());
    SoundMapEditorPtr = SoundMapEditor;
    SoundMapEditor->InitSoundMapEditor(Mode, InitToolkitHost, InSoundMapAsset);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSoundMapEditorModule, SoundMapEditor)