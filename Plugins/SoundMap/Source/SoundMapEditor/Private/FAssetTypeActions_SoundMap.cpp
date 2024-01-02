// Fill out your copyright notice in the Description page of Project Settings.


#include "FAssetTypeActions_SoundMap.h"

#include "SoundMapEditorModule.h"


void FAssetTypeActions_SoundMap::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                 TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Obj : InObjects)
	{
		USoundMapAsset* SoundMapAsset = Cast<USoundMapAsset>(Obj);
		if (SoundMapAsset != nullptr)
		{
			FSoundMapEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<FSoundMapEditorModule>("SoundMapEditor");
			EditorModule->CreateSoundMapEditor(Mode, EditWithinLevelEditor, SoundMapAsset);
		}
	}
}
