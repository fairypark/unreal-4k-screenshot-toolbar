// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#include "HighResScreenshotToolbarCommands.h"

#define LOCTEXT_NAMESPACE "FHighResScreenshotToolbarModule"

void FHighResScreenshotToolbarCommands::RegisterCommands()
{
	UI_COMMAND(
		PluginAction,
		"4K Shot",
		"Capture the active Level Editor viewport at 3840 x 2160.",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::F12));
}

#undef LOCTEXT_NAMESPACE
