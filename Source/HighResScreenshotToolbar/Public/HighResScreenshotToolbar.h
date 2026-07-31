// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FHighResScreenshotToolbarModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void TakeScreenshot();
	bool CanTakeScreenshot() const;
	void RegisterMenus();
	void RegisterSettings();
	void UnregisterSettings();

	TSharedPtr<class FUICommandList> PluginCommands;
};
