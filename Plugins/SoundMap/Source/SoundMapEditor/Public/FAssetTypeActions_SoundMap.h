// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "SoundMapAsset.h"
/**
 * 
 */
class SOUNDMAPEDITOR_API FAssetTypeActions_SoundMap : public FAssetTypeActions_Base 
{
public:
	FAssetTypeActions_SoundMap(EAssetTypeCategories::Type InAssetCategory)
		: AssetTypeCategory(InAssetCategory)
	{
		
	}

	virtual FText GetName() const override{return INVTEXT("SoundMap Asset");}
	virtual UClass* GetSupportedClass() const override{return USoundMapAsset::StaticClass();}
	virtual FColor GetTypeColor() const override{return FColor::Blue;}
	virtual uint32 GetCategories() override{return AssetTypeCategory;}
	
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	
	
private:
	EAssetTypeCategories::Type AssetTypeCategory;
	
};
