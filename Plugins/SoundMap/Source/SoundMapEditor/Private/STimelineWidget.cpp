// Fill out your copyright notice in the Description page of Project Settings.


#include "STimelineWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void STimelineWidget::Construct(const FArguments& InArgs)
{
	SoundMap = InArgs._SoundMap;
	SoundWave = InArgs._SoundWave;

	FText widgetText = SoundWave.Get() == nullptr ? FText::FromString("No Sound") : FText::FromString("Yes Sound");
	
	ChildSlot
	[
		SAssignNew(TextBlock,STextBlock).Text(widgetText).Text_Lambda([this]()->FText
		{
			const bool bHasSoundWave = SoundWave.Get() != nullptr;

            // Return appropriate text based on the condition
            return bHasSoundWave ? FText::FromString(TEXT("Yes Sound")) : FText::FromString(TEXT("No Sound"));
		})
	];
}

void STimelineWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (TextBlock.IsValid())
	{
		const bool bHasSoundWave = SoundWave.Get() != nullptr;
		
		TextBlock->SetText(bHasSoundWave ? FText::FromString(TEXT("Yes Sound")) : FText::FromString(TEXT("No Sound")));
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
