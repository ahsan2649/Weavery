// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
/**
 * 
 */
class SOUNDMAPEDITOR_API FSoundMapEditorCommands : public TCommands<FSoundMapEditorCommands>
{
public:
	FSoundMapEditorCommands();
	
	virtual void RegisterCommands() override;
	static const FSoundMapEditorCommands& Get();

	TSharedPtr<FUICommandInfo> PlaySoundWave;
	TSharedPtr<FUICommandInfo> PauseSoundWave;
	TSharedPtr<FUICommandInfo> StopSoundWave;
	TSharedPtr<FUICommandInfo> TogglePlayback;

	
};
