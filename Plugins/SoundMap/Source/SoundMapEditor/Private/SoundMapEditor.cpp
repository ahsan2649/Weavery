// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMapEditor.h"

#include "AudioDevice.h"
#include "AudioDeviceManager.h"
#include "SoundMapAsset.h"
#include "SoundMapEditorCommands.h"
#include "SparseSampledSequenceTransportCoordinator.h"
#include "ToolMenus.h"
#include "Styling/AppStyle.h"
#include "STimelineWidget.h"
#include "TransformedWaveformViewFactory.h"
#include "WaveformEditorZoomController.h"
#include "STransformedWaveformViewPanel.h"
#include "WaveformEditorSequenceDataProvider.h"



#define LOCTEXT_NAMESPACE "SoundMapEditor"


void FSoundMapEditor::InitSoundMapEditor(const EToolkitMode::Type Mode,
                                         const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                         USoundMapAsset* InSoundMapAsset)
{
	SoundMap = InSoundMapAsset;
	SoundMap->OnSoundWaveSet.BindSP(this, &FSoundMapEditor::OnSoundWaveSet);
	SoundMap->OnSoundWaveCleared.BindSP(this, &FSoundMapEditor::OnSoundWaveCleared);

	RegisterToolbar();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("StandaloneDefault")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split(
				FTabManager::NewStack()
				->SetHideTabWell(false)
				->AddTab(FName("Details"), ETabState::OpenedTab
				))
			->Split(
				FTabManager::NewStack()
				->SetHideTabWell(false)
				->AddTab(FName("Timeline"), ETabState::OpenedTab)
			)
		);

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		FName("SoundMapEditorAppIdentifier"),
		StandaloneDefaultLayout,
		true,
		true,
		InSoundMapAsset
	);
}

void FSoundMapEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("SoundMap Editor"));
	const TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FName("Details"),
	                                 FOnSpawnTab::CreateSP(this, &FSoundMapEditor::SpawnDetailsTab))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetDisplayName(INVTEXT("Details"));

	InTabManager->RegisterTabSpawner(FName("Timeline"),
	                                 FOnSpawnTab::CreateSP(this, &FSoundMapEditor::SpawnTimelineTab))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetDisplayName(INVTEXT("Timeline"));
}

void FSoundMapEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(FName("Details"));
	InTabManager->UnregisterTabSpawner(FName("Timeline"));
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

void FSoundMapEditor::RegisterToolbar()
{
	const FSoundMapEditorCommands& Commands = FSoundMapEditorCommands::Get();
	const FName MenuName = FAssetEditorToolkit::GetToolMenuToolbarName();

	if (!UToolMenus::Get()->IsMenuRegistered(MenuName))
	{
		UToolMenu* ToolBar = UToolMenus::Get()->RegisterMenu(MenuName, "AssetEditor.DefaultToolBar",
		                                                     EMultiBoxType::ToolBar);

		if (ToolBar == nullptr)
		{
			return;
		}

		FToolMenuInsert InsertAfterAssetSection("Asset", EToolMenuInsertType::After);
		FToolMenuSection& PlayBackSection = ToolBar->AddSection("Transport Controls", TAttribute<FText>(),
		                                                        InsertAfterAssetSection);

		FToolMenuEntry PlayEntry = FToolMenuEntry::InitToolBarButton(
			Commands.PlaySoundWave,
			LOCTEXT("SoundMapEditorPlayButton", ""),
			LOCTEXT("SoundMapEditorPlayButtonTooltip", "Plays this SoundWave"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.PlayInViewport")
		);

		PlayEntry.StyleNameOverride = FName("Toolbar.BackplateLeftPlay");

		FToolMenuEntry PauseEntry = FToolMenuEntry::InitToolBarButton(
			Commands.PauseSoundWave,
			LOCTEXT("SoundMapEditorPauseButton", ""),
			LOCTEXT("SoundMapEditorPauseButtonTooltip", "Pauses this SoundWave"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.PausePlaySession.Small")
		);

		PauseEntry.StyleNameOverride = FName("Toolbar.BackplateCenter");

		FToolMenuEntry StopEntry = FToolMenuEntry::InitToolBarButton(
			Commands.StopSoundWave,
			LOCTEXT("SoundMapEditorStopButton", ""),
			LOCTEXT("SoundMapEditorStopButtonTooltip", "Stops this SoundWave"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "PlayWorld.StopPlaySession.Small")
		);

		StopEntry.StyleNameOverride = FName("Toolbar.BackplateRight");

		PlayBackSection.AddEntry(PlayEntry);
		PlayBackSection.AddEntry(PauseEntry);
		PlayBackSection.AddEntry(StopEntry);
	}
}


TSharedRef<SDockTab> FSoundMapEditor::SpawnTimelineTab(const FSpawnTabArgs& SpawnTabArgs)
{
	SAssignNew(VerticalBox, SVerticalBox);
	
	return SNew(SDockTab)
	[
		VerticalBox.ToSharedRef()
	];
}

TSharedRef<SDockTab> FSoundMapEditor::SpawnDetailsTab(const FSpawnTabArgs& SpawnTabArgs) const
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	const TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{SoundMap});
	
	return SNew(SDockTab)
	[
		DetailsView
	];
}


bool FSoundMapEditor::CanPressPlayButton() const
{
	if (TransportController.Get() == nullptr) { return false; }
	return TransportController->CanPlay() && (TransportController->IsPaused() || !TransportController->IsPlaying());
}

void FSoundMapEditor::BindCommands()
{
	const FSoundMapEditorCommands& Commands = FSoundMapEditorCommands::Get();

	ToolkitCommands->MapAction(
		Commands.PlaySoundWave,
		FExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::Play),
		FCanExecuteAction::CreateSP(this, &FSoundMapEditor::CanPressPlayButton));

	ToolkitCommands->MapAction(
		Commands.StopSoundWave,
		FExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::Stop),
		FCanExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::CanStop));

	ToolkitCommands->MapAction(
		Commands.TogglePlayback,
		FExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::TogglePlayback)
	);

	ToolkitCommands->MapAction(
		Commands.PauseSoundWave,
		FExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::Pause),
		FCanExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::IsPlaying)
	);
}

void FSoundMapEditor::UnbindCommands() const
{
	const FSoundMapEditorCommands& Commands = FSoundMapEditorCommands::Get();
	ToolkitCommands->UnmapAction(Commands.PlaySoundWave);
	ToolkitCommands->UnmapAction(Commands.PauseSoundWave);
	ToolkitCommands->UnmapAction(Commands.StopSoundWave);
	ToolkitCommands->UnmapAction(Commands.TogglePlayback);
}

void FSoundMapEditor::InitializeAudioComponent()
{
	if (AudioComponent == nullptr)
	{
		if (const FAudioDeviceManager* AudioDeviceManager = FAudioDeviceManager::Get())
		{
			if (AudioDeviceManager->GetMainAudioDeviceRaw())
			{
				USoundBase* SoundBase = Cast<USoundBase>(SoundMap->SoundWave);
				AudioComponent = FAudioDevice::CreateComponent(SoundBase);
			}
		}
	}

	AudioComponent->bAutoDestroy = false;
	AudioComponent->bIsUISound = true;
	AudioComponent->bAllowSpatialization = false;
	AudioComponent->bReverb = false;
	AudioComponent->bCenterChannelOnly = false;
	AudioComponent->bIsPreviewSound = true;
}

void FSoundMapEditor::CreateTransportController()
{
	TransportController = MakeShared<FSoundMapTransportController>(AudioComponent);
}

void FSoundMapEditor::InitializeZoom()
{
	ZoomManager = MakeShared<FWaveformEditorZoomController>();
	check(TransportCoordinator);
	ZoomManager->OnZoomRatioChanged.AddSP(TransportCoordinator.ToSharedRef(),
	                                      &FSparseSampledSequenceTransportCoordinator::SetZoomRatio);
}

void FSoundMapEditor::HandleRenderDataUpdate()
{
	TransportCoordinator->UpdatePlaybackRange(WaveformView.DataProvider->GetTransformedWaveformBounds());
	WaveformView.DataProvider->RequestSequenceView(TransportCoordinator->GetDisplayRange());
}

void FSoundMapEditor::BindWaveformViewDelegates(FWaveformEditorSequenceDataProvider& ViewDataProvider,
                                                 STransformedWaveformViewPanel& ViewWidget)
{
	ViewDataProvider.OnRenderElementsUpdated.AddSP(this, &FSoundMapEditor::HandleRenderDataUpdate);
	TransportCoordinator->OnFocusPointMoved.AddSP(&ViewWidget, &STransformedWaveformViewPanel::SetPlayheadRatio);
}

void FSoundMapEditor::RemoveWaveformViewDelegates(FWaveformEditorSequenceDataProvider& ViewDataProvider,
                                                  STransformedWaveformViewPanel& ViewWidget)
{
	ViewDataProvider.OnRenderElementsUpdated.RemoveAll(this);
	TransportCoordinator->OnFocusPointMoved.RemoveAll(&ViewWidget);
}

void FSoundMapEditor::HandlePlaybackPercentageChange(const UAudioComponent* InComponent, const USoundWave* SoundWave,
	float InPlaybackPercentage)
{
	const bool bIsStopped = AudioComponent->GetPlayState() == EAudioComponentPlayState::Stopped;
	const bool bIsPaused = AudioComponent->GetPlayState() == EAudioComponentPlayState::Paused;
	const bool bPropagatePercentage = !bIsStopped && !bIsPaused;
	LastReceivedPlaybackPercent = InPlaybackPercentage;
	
	if (InComponent == AudioComponent && bPropagatePercentage)
	{
		if (TransportCoordinator.IsValid())
		{
			const float ClampedPlayBackPercentage = FGenericPlatformMath::Fmod(InPlaybackPercentage, 1.f);
			TransportCoordinator->SetProgressRatio(ClampedPlayBackPercentage);
		}
	}
}

void FSoundMapEditor::HandleAudioComponentPlayStateChanged(const UAudioComponent* InAudioComponent,
	EAudioComponentPlayState NewPlayState)
{
	if (InAudioComponent != AudioComponent)
	{
		return;
	}

	switch (NewPlayState)
	{
	default:
		break;
	case EAudioComponentPlayState::Stopped:
		const bool bIgnoreStopState = TransportCoordinator->IsScrubbing();

		if (!bIgnoreStopState)
		{
			TransportController->CacheStartTime(0.f);
			TransportCoordinator->Stop();
		}

		break;
	}
}

void FSoundMapEditor::HandlePlayheadScrub(float InTargetPlayBackRatio, bool bIsMoving)
{
	if (bIsMoving)
	{
		if (TransportController->IsPlaying())
		{
			TransportController->Stop();
			bWasPlayingBeforeScrubbing = true;
		}
	}
	else
	{
		const float NewTime = InTargetPlayBackRatio * SoundMap->SoundWave->Duration;

		if (TransportController->IsPlaying())
		{
			TransportController->Seek(NewTime);
			return;
		}

		if (bWasPlayingBeforeScrubbing)
		{
			TransportController->Play(NewTime);
			bWasPlayingBeforeScrubbing = false;
		}
		else
		{
			TransportController->CacheStartTime(NewTime);
		}

	}
}

void FSoundMapEditor::BindDelegates()
{
	AudioComponent->OnAudioPlaybackPercentNative.AddSP(this, &FSoundMapEditor::HandlePlaybackPercentageChange);
	AudioComponent->OnAudioPlayStateChangedNative.AddSP(this, &FSoundMapEditor::HandleAudioComponentPlayStateChanged);
	TransportCoordinator->OnFocusPointScrubUpdate.AddSP(this, &FSoundMapEditor::HandlePlayheadScrub);
}

void FSoundMapEditor::UnbindDelegates()
{
	AudioComponent->OnAudioPlaybackPercentNative.RemoveAll(this);
	AudioComponent->OnAudioPlayStateChangedNative.RemoveAll(this);
	TransportCoordinator->OnFocusPointScrubUpdate.RemoveAll(this);
}

void FSoundMapEditor::CreateWaveformView()
{
	auto fac = FTransformedWaveformViewFactory::Get();


	WaveformView = fac.GetTransformedView(
		SoundMap->SoundWave, TransportCoordinator.ToSharedRef(), this, ZoomManager);

	BindWaveformViewDelegates(*WaveformView.DataProvider, *WaveformView.ViewWidget);

	ZoomManager->OnZoomRatioChanged.AddSP(TransportCoordinator.ToSharedRef(),
	                                      &FSparseSampledSequenceTransportCoordinator::SetZoomRatio);
	TransportCoordinator->OnDisplayRangeUpdated.AddSP(this, &FSoundMapEditor::HandleDisplayRangeUpdate);

	VerticalBox->AddSlot().MaxHeight(150)[WaveformView.ViewWidget.ToSharedRef()];
}

void FSoundMapEditor::OnSoundWaveSet()
{
	CreateTransportCoordinator();
	InitializeZoom();
	InitializeAudioComponent();
	CreateTransportController();
	BindDelegates();
	BindCommands();
	CreateWaveformView();
}

void FSoundMapEditor::DestroyWaveformView()
{
	VerticalBox->RemoveSlot(WaveformView.ViewWidget.ToSharedRef());
	WaveformView.DataProvider.Reset();
	WaveformView.ViewWidget.Reset();
	
}

void FSoundMapEditor::UninitializeZoom()
{
	ZoomManager.Reset();
}

void FSoundMapEditor::DestroyTransportCoordinator()
{
	TransportCoordinator.Reset();
}

void FSoundMapEditor::DestroyAudioComponent()
{
	AudioComponent->DestroyComponent();
	AudioComponent = nullptr;
}

void FSoundMapEditor::HandleDisplayRangeUpdate(TRange<double> Range)
{
	WaveformView.DataProvider->RequestSequenceView(Range);
}

void FSoundMapEditor::CreateTransportCoordinator()
{
	TransportCoordinator = MakeShared<FSparseSampledSequenceTransportCoordinator>();
	TransportCoordinator->OnDisplayRangeUpdated.AddSP(this, &FSoundMapEditor::HandleDisplayRangeUpdate);
}

void FSoundMapEditor::DestroyTransportController()
{
	TransportController.Reset();
}

void FSoundMapEditor::OnSoundWaveCleared()
{
	DestroyWaveformView();
	UnbindCommands();
	UnbindDelegates();
	DestroyTransportController();
	DestroyAudioComponent();
	UninitializeZoom();
	DestroyTransportCoordinator();
}

FName FSoundMapEditor::GetToolkitFName() const
{
	return FName("SoundMapEditor");
}

FText FSoundMapEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("SoundMapEditor", "AppLabel", "SoundMap Asset Editor");
}

FString FSoundMapEditor::GetWorldCentricTabPrefix() const
{
	return NSLOCTEXT("SoundMapEditor", "WorldCentricTabPrefix", "SoundMapAsset").ToString();
}

FLinearColor FSoundMapEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Blue;
}

USoundMapAsset* FSoundMapEditor::GetSoundMap() const
{
	return SoundMap;
}

USoundWave* FSoundMapEditor::GetSoundWave() const
{
	return SoundMap->SoundWave;
}

void FSoundMapEditor::NotifyPreChange(FProperty* PropertyAboutToChange)
{
	FNotifyHook::NotifyPreChange(PropertyAboutToChange);
}

void FSoundMapEditor::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent,
                                       FProperty* PropertyThatChanged)
{
	FNotifyHook::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);
}

#undef LOCTEXT_NAMESPACE
