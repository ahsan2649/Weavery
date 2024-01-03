// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMapAsset.h"

void USoundMapAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(USoundMapAsset, SoundWave))
	{
		UE_LOG(LogTemp, Display, TEXT("SoundWave changed!"));
	}
	
}
