// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMapAssetFactory.h"

#include "SoundMapAsset.h"

USoundMapAssetFactory::USoundMapAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = false;
	SupportedClass = USoundMapAsset::StaticClass();
}

UObject* USoundMapAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                 UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<USoundMapAsset>(InParent, InClass, InName, Flags);
}
