// Fill out your copyright notice in the Description page of Project Settings.


#include "FSoundMapTransportController.h"



FSoundMapTransportController::FSoundMapTransportController(UAudioComponent* InAudioComponent)
	:AudioComponent(InAudioComponent)
{
}

FSoundMapTransportController::~FSoundMapTransportController()
{
	Stop();
}

void FSoundMapTransportController::Play()
{
	if (!CanPlay())
	{
		return;
	}
	if (IsPaused())
	{
		AudioComponent->SetPaused(false);
		if (!bCachedTimeDuringPause)
		{
			return;
		}
	}

	AudioComponent->Play(CachedAudioStartTime);
}

void FSoundMapTransportController::Play(const float StartTime)
{
	if (!CanPlay())
	{
		return;
	}

	if (IsPaused())
	{
		AudioComponent->SetPaused(false);
	}

	CacheStartTime(StartTime);
	AudioComponent->Play(CachedAudioStartTime);
}

void FSoundMapTransportController::Pause()
{
	AudioComponent->SetPaused(true);
}

void FSoundMapTransportController::Stop()
{
	if (!CanStop())
	{
		return;
	}

	Play(0.f);
	AudioComponent->StopDelayed(0.1f);

	if (IsPaused())
	{
		AudioComponent->SetPaused(false);
	}

	bCachedTimeDuringPause = false;
}

void FSoundMapTransportController::TogglePlayback()
{
	if (IsPlaying())
	{
		Pause();
	}
	else
	{
		Play();
	}
}

bool FSoundMapTransportController::CanPlay() const
{
	return SoundBaseIsValid();
}

bool FSoundMapTransportController::CanStop() const
{
	return (IsPlaying() || IsPaused());
}

bool FSoundMapTransportController::IsPaused() const
{
	return AudioComponent->GetPlayState() == EAudioComponentPlayState::Paused;
}

bool FSoundMapTransportController::IsPlaying() const
{
	return AudioComponent->GetPlayState() == EAudioComponentPlayState::Playing;

}

void FSoundMapTransportController::CacheStartTime(const float StartTime)
{
	CachedAudioStartTime = StartTime;

	if (IsPaused())
	{
		bCachedTimeDuringPause = true;
	}
	else
	{
		bCachedTimeDuringPause = false;
	}
}

void FSoundMapTransportController::Seek(const float SeekTime)
{
	AudioComponent->Play(SeekTime);
}

const bool FSoundMapTransportController::SoundBaseIsValid() const
{
	return AudioComponent->GetSound() != nullptr;
}


