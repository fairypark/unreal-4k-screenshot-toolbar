// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#pragma once

#include "Framework/Commands/Commands.h"
#include "HighResScreenshotToolbarStyle.h"

class FHighResScreenshotToolbarCommands : public TCommands<FHighResScreenshotToolbarCommands>
{
public:

	FHighResScreenshotToolbarCommands()
		: TCommands<FHighResScreenshotToolbarCommands>(TEXT("HighResScreenshotToolbar"), NSLOCTEXT("Contexts", "HighResScreenshotToolbar", "HighResScreenshotToolbar Plugin"), NAME_None, FHighResScreenshotToolbarStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
