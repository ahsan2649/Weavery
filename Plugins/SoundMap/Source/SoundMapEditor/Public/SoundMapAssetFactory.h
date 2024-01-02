// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "SoundMapAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class SOUNDMAPEDITOR_API USoundMapAssetFactory : public UFactory
{
	GENERATED_BODY()

	USoundMapAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
