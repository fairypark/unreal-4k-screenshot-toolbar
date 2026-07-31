// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#include "HighResScreenshotToolbar.h"
#include "HighResScreenshotToolbarCommands.h"
#include "HighResScreenshotToolbarSettings.h"
#include "HighResScreenshotToolbarStyle.h"

#include "Editor.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HighResScreenshot.h"
#include "ISettingsModule.h"
#include "LevelEditor.h"
#include "LevelEditorViewport.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"
#include "ToolMenus.h"
#include "UnrealClient.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FHighResScreenshotToolbarModule"

namespace HighResScreenshotToolbar
{
	FDelegateHandle ScreenshotCapturedHandle;
	FDelegateHandle ScreenshotProcessedHandle;
	bool bWaitingForScreenshot = false;
	FString PendingScreenshotPath;
	FLevelEditorViewportClient* PendingViewportClient = nullptr;
	bool bChangedGameView = false;
	bool bPreviousShowWidget = true;

	void RestoreViewportState()
	{
		if (bChangedGameView && PendingViewportClient != nullptr && GEditor != nullptr
			&& GEditor->GetLevelViewportClients().Contains(PendingViewportClient))
		{
			PendingViewportClient->SetGameView(false);
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
			PendingViewportClient->ShowWidget(bPreviousShowWidget);
#endif
			PendingViewportClient->Invalidate();
		}

		PendingViewportClient = nullptr;
		bChangedGameView = false;
		bPreviousShowWidget = true;
	}

	void HandleScreenshotCaptured(int32, int32, const TArray<FColor>&)
	{
		if (!bWaitingForScreenshot)
		{
			return;
		}

		PendingScreenshotPath = FPaths::ConvertRelativePathToFull(FScreenshotRequest::GetFilename());
	}

	void HandleScreenshotProcessed()
	{
		if (!bWaitingForScreenshot)
		{
			return;
		}

		bWaitingForScreenshot = false;
		RestoreViewportState();
		const bool bSaved = !PendingScreenshotPath.IsEmpty() && FPaths::FileExists(PendingScreenshotPath);

		if (!bSaved)
		{
			FNotificationInfo Info(LOCTEXT("CaptureWriteFailed", "Screenshot failed: the output file was not created."));
			Info.bFireAndForget = true;
			Info.ExpireDuration = 10.0f;
			Info.bUseSuccessFailIcons = true;
			Info.bUseLargeFont = true;

			if (TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
			{
				Notification->SetCompletionState(SNotificationItem::CS_Fail);
			}
		}

		PendingScreenshotPath.Reset();
	}

	void RegisterScreenshotDelegates()
	{
		if (!ScreenshotCapturedHandle.IsValid())
		{
			ScreenshotCapturedHandle = FScreenshotRequest::OnScreenshotCaptured().AddStatic(&HandleScreenshotCaptured);
		}

		if (!ScreenshotProcessedHandle.IsValid())
		{
			ScreenshotProcessedHandle = FScreenshotRequest::OnScreenshotRequestProcessed().AddStatic(&HandleScreenshotProcessed);
		}
	}

	void UnregisterScreenshotDelegates()
	{
		if (ScreenshotCapturedHandle.IsValid())
		{
			FScreenshotRequest::OnScreenshotCaptured().Remove(ScreenshotCapturedHandle);
			ScreenshotCapturedHandle.Reset();
		}

		if (ScreenshotProcessedHandle.IsValid())
		{
			FScreenshotRequest::OnScreenshotRequestProcessed().Remove(ScreenshotProcessedHandle);
			ScreenshotProcessedHandle.Reset();
		}

		bWaitingForScreenshot = false;
		PendingScreenshotPath.Reset();
		RestoreViewportState();
	}
}

void FHighResScreenshotToolbarModule::StartupModule()
{
	FHighResScreenshotToolbarStyle::Initialize();
	FHighResScreenshotToolbarStyle::ReloadTextures();
	FHighResScreenshotToolbarCommands::Register();
	HighResScreenshotToolbar::RegisterScreenshotDelegates();
	RegisterSettings();

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	PluginCommands = LevelEditorModule.GetGlobalLevelEditorActions();

	PluginCommands->MapAction(
		FHighResScreenshotToolbarCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FHighResScreenshotToolbarModule::TakeScreenshot),
		FCanExecuteAction::CreateRaw(this, &FHighResScreenshotToolbarModule::CanTakeScreenshot));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHighResScreenshotToolbarModule::RegisterMenus));
}

void FHighResScreenshotToolbarModule::ShutdownModule()
{
	HighResScreenshotToolbar::UnregisterScreenshotDelegates();
	UnregisterSettings();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	if (PluginCommands.IsValid())
	{
		PluginCommands->UnmapAction(FHighResScreenshotToolbarCommands::Get().PluginAction);
		PluginCommands.Reset();
	}
	FHighResScreenshotToolbarStyle::Shutdown();
	FHighResScreenshotToolbarCommands::Unregister();
}

void FHighResScreenshotToolbarModule::TakeScreenshot()
{
	FLevelEditorViewportClient* ViewportClient = GCurrentLevelEditingViewportClient;
	if (ViewportClient == nullptr)
	{
		FNotificationInfo Info(LOCTEXT("NoActiveViewport", "Screenshot failed: no active Level Editor viewport."));
		Info.ExpireDuration = 4.0f;
		if (TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
		{
			Notification->SetCompletionState(SNotificationItem::CS_Fail);
		}
		return;
	}

	const FIntPoint CaptureResolution =
		GetDefault<UHighResScreenshotToolbarSettings>()->GetCaptureResolution();

	// Reset optional screenshot flags and request the configured exact PNG resolution.
	const FString CaptureArguments = FString::Printf(
		TEXT("%dx%d 0 0 0 0 0 0 0 1"),
		CaptureResolution.X,
		CaptureResolution.Y);
	if (!GetHighResScreenshotConfig().ParseConsoleCommand(CaptureArguments, *GLog))
	{
		FNotificationInfo Info(LOCTEXT("InvalidCaptureConfig", "Screenshot failed: the configured resolution was rejected."));
		Info.ExpireDuration = 4.0f;
		if (TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
		{
			Notification->SetCompletionState(SNotificationItem::CS_Fail);
		}
		return;
	}

	HighResScreenshotToolbar::bWaitingForScreenshot = true;
	HighResScreenshotToolbar::PendingScreenshotPath.Reset();
	HighResScreenshotToolbar::PendingViewportClient = ViewportClient;
	HighResScreenshotToolbar::bChangedGameView = !ViewportClient->IsInGameView();
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1)
	HighResScreenshotToolbar::bPreviousShowWidget = ViewportClient->GetShowWidget();
#endif
	if (HighResScreenshotToolbar::bChangedGameView)
	{
		ViewportClient->SetGameView(true);
	}
	ViewportClient->TakeHighResScreenShot();
}

bool FHighResScreenshotToolbarModule::CanTakeScreenshot() const
{
	return GCurrentLevelEditingViewportClient != nullptr && !HighResScreenshotToolbar::bWaitingForScreenshot;
}

void FHighResScreenshotToolbarModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::LoadModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->RegisterSettings(
			TEXT("Editor"),
			TEXT("Plugins"),
			TEXT("HighResScreenshotToolbar"),
			LOCTEXT("SettingsName", "4K Screenshot Toolbar"),
			LOCTEXT(
				"SettingsDescription",
				"Choose a capture resolution preset or enter a custom width and height."),
			GetMutableDefault<UHighResScreenshotToolbarSettings>());
	}
}

void FHighResScreenshotToolbarModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->UnregisterSettings(
			TEXT("Editor"),
			TEXT("Plugins"),
			TEXT("HighResScreenshotToolbar"));
	}
}

void FHighResScreenshotToolbarModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("CaptureTools");
	FToolMenuEntry& Entry = Section.AddEntry(
		FToolMenuEntry::InitToolBarButton(FHighResScreenshotToolbarCommands::Get().PluginAction));
	Entry.SetCommandList(PluginCommands);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHighResScreenshotToolbarModule, HighResScreenshotToolbar)
