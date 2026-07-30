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
	void Take4KScreenshot();
	bool CanTake4KScreenshot() const;
	void RegisterMenus();

	TSharedPtr<class FUICommandList> PluginCommands;
};
