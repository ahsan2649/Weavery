#include "SoundMapEditorModule.h"

#include "AssetToolsModule.h"
#include "FAssetTypeActions_SoundMap.h"
#include "IAssetTools.h"

#define LOCTEXT_NAMESPACE "FSoundMapEditorModule"

void FSoundMapEditorModule::StartupModule()
{
    IModuleInterface::StartupModule();

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    AssetTypeCategory = AssetTools.RegisterAdvancedAssetCategory("SoundMap Plugin", INVTEXT("SoundMap Plugin"));

    TSharedRef<IAssetTypeActions> Actions = MakeShared<FAssetTypeActions_SoundMap>(AssetTypeCategory);
    AssetTools.RegisterAssetTypeActions(Actions);
    CreatedAssetTypeActions = Actions;
    
    
}

void FSoundMapEditorModule::ShutdownModule()
{
    IModuleInterface::ShutdownModule();

    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions.ToSharedRef());
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSoundMapEditorModule, SoundMapEditor)