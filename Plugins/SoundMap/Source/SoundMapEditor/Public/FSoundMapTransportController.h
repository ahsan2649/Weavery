// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"

/**
 * 
 */
class SOUNDMAPEDITOR_API FSoundMapTransportController
{
public:
	explicit FSoundMapTransportController(UAudioComponent* InAudioComponent);
	~FSoundMapTransportController();

	void Play();
	void Play(const float StartTime);
	void Pause();
	void Stop();
	void TogglePlayback();
	bool CanPlay() const;
	bool CanStop() const;
	bool IsPaused() const;
	bool IsPlaying() const;
	void CacheStartTime(const float StartTime);
	void Seek(const float SeekTime);

private:
	const bool SoundBaseIsValid() const;

	UAudioComponent* AudioComponent = nullptr;
	float CachedAudioStartTime = 0.f;
	bool bCachedTimeDuringPause = false;

	FDelegateHandle PlayStateChangeDelegateHandle;
};
