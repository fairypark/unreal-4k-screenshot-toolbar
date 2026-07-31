// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#include "HighResScreenshotToolbarSettings.h"

namespace HighResScreenshotToolbarSettings
{
	constexpr int32 MinCaptureDimension = 64;
	constexpr int32 MaxCaptureDimension = 16384;
}

UHighResScreenshotToolbarSettings::UHighResScreenshotToolbarSettings()
	: ResolutionPreset(EHighResScreenshotResolutionPreset::UHD4K)
	, CustomWidth(3840)
	, CustomHeight(2160)
{
}

FIntPoint UHighResScreenshotToolbarSettings::GetCaptureResolution() const
{
	switch (ResolutionPreset)
	{
	case EHighResScreenshotResolutionPreset::FullHD:
		return FIntPoint(1920, 1080);
	case EHighResScreenshotResolutionPreset::QHD:
		return FIntPoint(2560, 1440);
	case EHighResScreenshotResolutionPreset::UltrawideQHD:
		return FIntPoint(3440, 1440);
	case EHighResScreenshotResolutionPreset::FiveK:
		return FIntPoint(5120, 2880);
	case EHighResScreenshotResolutionPreset::UHD8K:
		return FIntPoint(7680, 4320);
	case EHighResScreenshotResolutionPreset::Custom:
		return FIntPoint(
			FMath::Clamp(
				CustomWidth,
				HighResScreenshotToolbarSettings::MinCaptureDimension,
				HighResScreenshotToolbarSettings::MaxCaptureDimension),
			FMath::Clamp(
				CustomHeight,
				HighResScreenshotToolbarSettings::MinCaptureDimension,
				HighResScreenshotToolbarSettings::MaxCaptureDimension));
	case EHighResScreenshotResolutionPreset::UHD4K:
	default:
		return FIntPoint(3840, 2160);
	}
}
