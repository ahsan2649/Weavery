#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"

class FSoundMapEditor;
class USoundMapAsset;
class IAssetTypeActions;


class FSoundMapEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    void CreateSoundMapEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USoundMapAsset* InSoundMapAsset);
private:
    EAssetTypeCategories::Type AssetTypeCategory;
    TSharedPtr<IAssetTypeActions> CreatedAssetTypeActions;
    TWeakPtr<FSoundMapEditor> SoundMapEditorPtr;
};
