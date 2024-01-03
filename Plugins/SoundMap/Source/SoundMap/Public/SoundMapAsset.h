// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoundMapAsset.generated.h"

/**
 * 
 */
USTRUCT()
struct FRail
{
	GENERATED_BODY()
	TArray<float> Stamps;
};

UCLASS()
class SOUNDMAP_API USoundMapAsset : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	USoundWave* SoundWave;

	UPROPERTY(EditAnywhere)
	TArray<FRail> Rails;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
