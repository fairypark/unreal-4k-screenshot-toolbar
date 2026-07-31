// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "HighResScreenshotToolbarSettings.generated.h"

UENUM()
enum class EHighResScreenshotResolutionPreset : uint8
{
	FullHD UMETA(DisplayName = "Full HD (1920 x 1080)"),
	QHD UMETA(DisplayName = "QHD (2560 x 1440)"),
	UltrawideQHD UMETA(DisplayName = "Ultrawide QHD (3440 x 1440)"),
	UHD4K UMETA(DisplayName = "4K UHD (3840 x 2160)"),
	FiveK UMETA(DisplayName = "5K (5120 x 2880)"),
	UHD8K UMETA(DisplayName = "8K UHD (7680 x 4320)"),
	Custom UMETA(DisplayName = "Custom")
};

UCLASS(config = EditorPerProjectUserSettings, defaultconfig)
class HIGHRESSCREENSHOTTOOLBAR_API UHighResScreenshotToolbarSettings : public UObject
{
	GENERATED_BODY()

public:
	UHighResScreenshotToolbarSettings();

	FIntPoint GetCaptureResolution() const;

	UPROPERTY(
		config,
		EditAnywhere,
		Category = "Capture Resolution",
		meta = (ToolTip = "Choose a standard capture size or Custom for an exact width and height."))
	EHighResScreenshotResolutionPreset ResolutionPreset;

	UPROPERTY(
		config,
		EditAnywhere,
		Category = "Capture Resolution",
		meta = (
			ClampMin = "64",
			UIMin = "64",
			ClampMax = "16384",
			UIMax = "16384",
			EditCondition = "ResolutionPreset == EHighResScreenshotResolutionPreset::Custom",
			ToolTip = "PNG width used when Resolution Preset is Custom."))
	int32 CustomWidth;

	UPROPERTY(
		config,
		EditAnywhere,
		Category = "Capture Resolution",
		meta = (
			ClampMin = "64",
			UIMin = "64",
			ClampMax = "16384",
			UIMax = "16384",
			EditCondition = "ResolutionPreset == EHighResScreenshotResolutionPreset::Custom",
			ToolTip = "PNG height used when Resolution Preset is Custom."))
	int32 CustomHeight;
};
