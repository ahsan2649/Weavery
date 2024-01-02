// Fill out your copyright notice in the Description page of Project Settings.


#include "FAssetTypeActions_SoundMap.h"


void FAssetTypeActions_SoundMap::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}
