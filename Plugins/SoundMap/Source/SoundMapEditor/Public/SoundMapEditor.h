// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSoundMapTransportController.h"
#include "SoundMapEditorZoomController.h"
#include "TransformedWaveformView.h"



class STimelineWidget;
class USoundMapAsset;
/**
 * 
 */
class SOUNDMAPEDITOR_API FSoundMapEditor : public FAssetEditorToolkit, public FNotifyHook
{
public:
	void InitSoundMapEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, USoundMapAsset* InSoundMapAsset);

	// FAssetEditorToolkit stuff
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	// Getters for UObjects
	USoundMapAsset* GetSoundMap() const;
	USoundWave* GetSoundWave() const;

	virtual void NotifyPreChange(FProperty* PropertyAboutToChange) override;
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, FProperty* PropertyThatChanged) override;
	void HandleRenderDataUpdate();
	
private:

	// SoundMap
	TObjectPtr<USoundMapAsset> SoundMap = nullptr;
	void InitializeZoom();
	void BindWaveformViewDelegates(FWaveformEditorSequenceDataProvider& DataProvider,
	                                STransformedWaveformViewPanel& ViewWidget);
	void RemoveWaveformViewDelegates(FWaveformEditorSequenceDataProvider& ViewDataProvider, STransformedWaveformViewPanel& ViewWidget);

	void HandlePlaybackPercentageChange(const UAudioComponent* AudioComponent, const USoundWave* SoundWave, float X);
	void HandleAudioComponentPlayStateChanged(const UAudioComponent* AudioComponent, EAudioComponentPlayState AudioComponentPlayState);
	void HandlePlayheadScrub(float X, bool bArg);
	void BindDelegates();
	void UnbindDelegates();
	void CreateWaveformView();
	void OnSoundWaveSet();
	void DestroyWaveformView();
	void UninitializeZoom();
	void DestroyTransportCoordinator();
	void OnSoundWaveCleared();

	// AudioComponent
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;
	void InitializeAudioComponent();
	void CreateTransportController();
	void DestroyAudioComponent();

	FTransformedWaveformView WaveformView;
	TSharedPtr<FSparseSampledSequenceTransportCoordinator> TransportCoordinator = nullptr;
	void HandleDisplayRangeUpdate(TRange<double> Range);
	void CreateTransportCoordinator();
	void DestroyTransportController();

	TSharedPtr<FSoundMapTransportController> TransportController = nullptr;
	TSharedPtr<FWaveformEditorZoomController> ZoomManager = nullptr;

	// Tab References
	TSharedRef<SDockTab> SpawnTimelineTab(const FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnDetailsTab(const FSpawnTabArgs& SpawnTabArgs) const;

	TSharedPtr<SVerticalBox> VerticalBox;
	// Toolbar Setup
	void RegisterToolbar();
	bool CanPressPlayButton() const;
	void BindCommands();
	void UnbindCommands() const;

	float LastReceivedPlaybackPercent = 0.f;
	bool bWasPlayingBeforeScrubbing = false;

	TSharedPtr<STimelineWidget> TimelineWidget;
	
};
