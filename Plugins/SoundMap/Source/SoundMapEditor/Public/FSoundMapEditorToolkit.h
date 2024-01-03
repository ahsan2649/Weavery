// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class STimelineWidget;
class USoundMapAsset;
/**
 * 
 */
class SOUNDMAPEDITOR_API FSoundMapEditorToolkit : public FAssetEditorToolkit
{
public:
	void InitSoundMapEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USoundMapAsset* InSoundMapAsset);
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	USoundMapAsset* GetSoundMap() const;
	USoundWave* GetSoundWave() const;

	
private:
	USoundMapAsset* SoundMap = nullptr;
	
	TSharedRef<SDockTab> SpawnTimelineTab(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<STimelineWidget> TimelineWidget;
		
};
