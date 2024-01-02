#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"

class USoundMapAsset;
class IAssetTypeActions;


class FSoundMapEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
private:
    EAssetTypeCategories::Type AssetTypeCategory;
    TSharedPtr<IAssetTypeActions> CreatedAssetTypeActions;
};
