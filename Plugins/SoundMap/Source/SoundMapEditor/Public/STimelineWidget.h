// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class USoundMapAsset;
/**
 * 
 */
class SOUNDMAPEDITOR_API STimelineWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STimelineWidget)
		{
		}
	SLATE_ATTRIBUTE(USoundWave*, SoundWave)
	SLATE_ATTRIBUTE(USoundMapAsset*, SoundMap)
	

	SLATE_END_ARGS()

	TAttribute<USoundMapAsset*> SoundMap;
	TAttribute<USoundWave*> SoundWave;
	
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

};
