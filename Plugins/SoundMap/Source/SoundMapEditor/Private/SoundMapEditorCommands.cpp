// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMapEditorCommands.h"

#define LOCTEXT_NAMESPACE "SoundMapEditorCommands"


FSoundMapEditorCommands::FSoundMapEditorCommands()
	: TCommands<FSoundMapEditorCommands>(
		  TEXT("SoundMapEditor"),
		  NSLOCTEXT("Contexts", "SoundMapEditor", "SoundMap Editor"),
		  NAME_None,
		  "AudioStyleSet")
{
}

void FSoundMapEditorCommands::RegisterCommands()
{
	UI_COMMAND(PlaySoundWave, "Play", "Play this sound wave", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PauseSoundWave, "Pause", "Pause this sound wave", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(StopSoundWave, "Stop", "Stops playback and rewinds to beginning of file", EUserInterfaceActionType::Button, FInputChord(EKeys::Enter));
	UI_COMMAND(TogglePlayback, "TogglePlayback", "Toggles between play and pause", EUserInterfaceActionType::Button, FInputChord(EKeys::SpaceBar));
}

const FSoundMapEditorCommands& FSoundMapEditorCommands::Get()
{
	return TCommands<FSoundMapEditorCommands>::Get();
}

#undef LOCTEXT_NAMESPACE
