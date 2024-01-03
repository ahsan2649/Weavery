// Fill out your copyright notice in the Description page of Project Settings.


#include "FSoundMapEditorToolkit.h"
#include "SoundMapAsset.h"
#include "STimelineWidget.h"

TSharedRef<SDockTab> FSoundMapEditorToolkit::SpawnTimelineTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
	[
		SAssignNew(TimelineWidget, STimelineWidget)
		.SoundMap(this, &FSoundMapEditorToolkit::GetSoundMap)
		.SoundWave(this, &FSoundMapEditorToolkit::GetSoundWave)
	];
}

TSharedRef<SDockTab> FSoundMapEditorToolkit::SpawnDetailsTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(
		"PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{SoundMap});

	return SNew(SDockTab)
	[
		DetailsView
	];
}

void FSoundMapEditorToolkit::InitSoundMapEditor(const EToolkitMode::Type Mode,
                                                const TSharedPtr<IToolkitHost>& InitToolkitHost,
                                                USoundMapAsset* InSoundMapAsset)
{
	SoundMap = InSoundMapAsset;
	
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
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
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
