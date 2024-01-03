// Fill out your copyright notice in the Description page of Project Settings.


#include "STimelineWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void STimelineWidget::Construct(const FArguments& InArgs)
{
	SoundMap = InArgs._SoundMap;
	SoundWave = InArgs._SoundWave;
	
	ChildSlot[
		SNew(SVerticalBox)
		+SVerticalBox::Slot().FillHeight(1)[SNew(STextBlock).Text(FText::FromString("Waveform goes here"))]
		+SVerticalBox::Slot().FillHeight(2)[SNew(STextBlock).Text(FText::FromString("Rails go here"))]
		];
}



END_SLATE_FUNCTION_BUILD_OPTIMIZATION
