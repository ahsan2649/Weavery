// Fill out your copyright notice in the Description page of Project Settings.


#include "STimelineWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void STimelineWidget::Construct(const FArguments& InArgs)
{
	SoundMap = InArgs._SoundMap;
	SoundWave = InArgs._SoundWave;

	
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
