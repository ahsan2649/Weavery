// Fill out your copyright notice in the Description page of Project Settings.


#include "FSoundMapEditorToolkit.h"

#include "AudioDevice.h"
#include "AudioDeviceManager.h"
#include "SoundMapAsset.h"
#include "SoundMapEditorCommands.h"
#include "ToolMenus.h"
#include "Styling/AppStyle.h"
#include "STimelineWidget.h"

#define LOCTEXT_NAMESPACE "SoundMapEditorCommands"


TSharedRef<SDockTab> FSoundMapEditorToolkit::SpawnTimelineTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
	[
		SAssignNew(TimelineWidget, STimelineWidget)
		.SoundMap(this, &FSoundMapEditorToolkit::GetSoundMap)
		.SoundWave(this, &FSoundMapEditorToolkit::GetSoundWave)
	];
}

TSharedRef<SDockTab> FSoundMapEditorToolkit::SpawnDetailsTab(const FSpawnTabArgs& SpawnTabArgs) const
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

void FSoundMapEditorToolkit::RegisterToolbar()
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

bool FSoundMapEditorToolkit::CanPressPlayButton() const
{
	if (TransportController.Get() == nullptr) { return false; }
	return TransportController->CanPlay() && (TransportController->IsPaused() || !TransportController->IsPlaying());
}

void FSoundMapEditorToolkit::BindCommands()
{
	const FSoundMapEditorCommands& Commands = FSoundMapEditorCommands::Get();

	ToolkitCommands->MapAction(
		Commands.PlaySoundWave,
		FExecuteAction::CreateSP(TransportController.ToSharedRef(), &FSoundMapTransportController::Play),
		FCanExecuteAction::CreateSP(this, &FSoundMapEditorToolkit::CanPressPlayButton));

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

void FSoundMapEditorToolkit::UnbindCommands() const
{
	const FSoundMapEditorCommands& Commands = FSoundMapEditorCommands::Get();
	ToolkitCommands->UnmapAction(Commands.PlaySoundWave);
	ToolkitCommands->UnmapAction(Commands.PauseSoundWave);
	ToolkitCommands->UnmapAction(Commands.StopSoundWave);
	ToolkitCommands->UnmapAction(Commands.TogglePlayback);
}

void FSoundMapEditorToolkit::InitializeAudioComponent()
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

void FSoundMapEditorToolkit::OnSoundWaveSet()
{
	InitializeAudioComponent();
	TransportController = MakeShared<FSoundMapTransportController>(AudioComponent);
	BindCommands();
}

void FSoundMapEditorToolkit::DestroyAudioComponent()
{
	AudioComponent->DestroyComponent();
	AudioComponent = nullptr;
}

void FSoundMapEditorToolkit::OnSoundWaveCleared()
{
	UnbindCommands();
	TransportController.Reset();
	DestroyAudioComponent();
}

void FSoundMapEditorToolkit::InitSoundMapEditor(const EToolkitMode::Type Mode,
                                                const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                USoundMapAsset* InSoundMapAsset)
{
	SoundMap = InSoundMapAsset;
	SoundMap->OnSoundWaveSet.BindSP(this, &FSoundMapEditorToolkit::OnSoundWaveSet);
	SoundMap->OnSoundWaveCleared.BindSP(this, &FSoundMapEditorToolkit::OnSoundWaveCleared);

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

void FSoundMapEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("SoundMap Editor"));
	const TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FName("Details"),
	                                 FOnSpawnTab::CreateSP(this, &FSoundMapEditorToolkit::SpawnDetailsTab))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetDisplayName(INVTEXT("Details"));

	InTabManager->RegisterTabSpawner(FName("Timeline"),
	                                 FOnSpawnTab::CreateSP(this, &FSoundMapEditorToolkit::SpawnTimelineTab))
	            .SetGroup(WorkspaceMenuCategoryRef)
	            .SetDisplayName(INVTEXT("Timeline"));
}

void FSoundMapEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(FName("Details"));
	InTabManager->UnregisterTabSpawner(FName("Timeline"));
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

FName FSoundMapEditorToolkit::GetToolkitFName() const
{
	return FName("SoundMapEditor");
}

FText FSoundMapEditorToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("SoundMapEditor", "AppLabel", "SoundMap Asset Editor");
}

FString FSoundMapEditorToolkit::GetWorldCentricTabPrefix() const
{
	return NSLOCTEXT("SoundMapEditor", "WorldCentricTabPrefix", "SoundMapAsset").ToString();
}

FLinearColor FSoundMapEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor::Blue;
}

USoundMapAsset* FSoundMapEditorToolkit::GetSoundMap() const
{
	return SoundMap;
}

USoundWave* FSoundMapEditorToolkit::GetSoundWave() const
{
	return SoundMap->SoundWave;
}

#undef LOCTEXT_NAMESPACE
