// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundMapEditorZoomController.h"


void FSoundMapEditorZoomController::ZoomIn()
{
	if (CanZoomIn())
	{
		ZoomLevel = FMath::Clamp(ZoomLevel += ZoomLevelStep, 0, LogRatioBase);
		ApplyZoom();
	}
}

bool FSoundMapEditorZoomController::CanZoomIn() const
{
	return ZoomLevel + ZoomLevelStep <= LogRatioBase + ZoomLevelInitValue;
}

void FSoundMapEditorZoomController::ZoomOut()
{
	if (CanZoomIn())
	{
		ZoomLevel = FMath::Clamp(ZoomLevel -= ZoomLevelStep, 0, LogRatioBase);
		ApplyZoom();
	}
}

bool FSoundMapEditorZoomController::CanZoomOut() const
{
	return ZoomLevel - ZoomLevelStep >= 0;
}

void FSoundMapEditorZoomController::ZoomByDelta(const float Delta)
{
	if (Delta>=0.f)
	{
		ZoomIn();
	}
	else
	{
		ZoomOut();
	}
}

float FSoundMapEditorZoomController::GetZoomRatio() const
{
	return 1-ConvertZoomLevelToLogRatio();
}

void FSoundMapEditorZoomController::ApplyZoom()
{
	OnZoomRatioChanged.Broadcast(1 - ConvertZoomLevelToLogRatio());
}

float FSoundMapEditorZoomController::ConvertZoomLevelToLogRatio() const
{
	return FMath::Clamp(FMath::LogX(LogRatioBase, ZoomLevel), 0.f, 1.f);
}




