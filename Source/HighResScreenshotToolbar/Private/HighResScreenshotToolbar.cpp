// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#include "HighResScreenshotToolbar.h"
#include "HighResScreenshotToolbarCommands.h"
#include "HighResScreenshotToolbarStyle.h"

#include "Editor.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HighResScreenshot.h"
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
	bool bWaitingFor4KShot = false;
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
		if (!bWaitingFor4KShot)
		{
			return;
		}

		PendingScreenshotPath = FPaths::ConvertRelativePathToFull(FScreenshotRequest::GetFilename());
	}

	void HandleScreenshotProcessed()
	{
		if (!bWaitingFor4KShot)
		{
			return;
		}

		bWaitingFor4KShot = false;
		RestoreViewportState();
		const bool bSaved = !PendingScreenshotPath.IsEmpty() && FPaths::FileExists(PendingScreenshotPath);

		if (!bSaved)
		{
			FNotificationInfo Info(LOCTEXT("CaptureWriteFailed", "4K Shot failed: the output file was not created."));
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

		bWaitingFor4KShot = false;
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

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	PluginCommands = LevelEditorModule.GetGlobalLevelEditorActions();

	PluginCommands->MapAction(
		FHighResScreenshotToolbarCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FHighResScreenshotToolbarModule::Take4KScreenshot),
		FCanExecuteAction::CreateRaw(this, &FHighResScreenshotToolbarModule::CanTake4KScreenshot));

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FHighResScreenshotToolbarModule::RegisterMenus));
}

void FHighResScreenshotToolbarModule::ShutdownModule()
{
	HighResScreenshotToolbar::UnregisterScreenshotDelegates();
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

void FHighResScreenshotToolbarModule::Take4KScreenshot()
{
	FLevelEditorViewportClient* ViewportClient = GCurrentLevelEditingViewportClient;
	if (ViewportClient == nullptr)
	{
		FNotificationInfo Info(LOCTEXT("NoActiveViewport", "4K Shot failed: no active Level Editor viewport."));
		Info.ExpireDuration = 4.0f;
		if (TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
		{
			Notification->SetCompletionState(SNotificationItem::CS_Fail);
		}
		return;
	}

	// Reset optional screenshot flags and request an exact 3840 x 2160 PNG.
	const FString CaptureArguments(TEXT("3840x2160 0 0 0 0 0 0 0 1"));
	if (!GetHighResScreenshotConfig().ParseConsoleCommand(CaptureArguments, *GLog))
	{
		FNotificationInfo Info(LOCTEXT("InvalidCaptureConfig", "4K Shot failed: the screenshot configuration was rejected."));
		Info.ExpireDuration = 4.0f;
		if (TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info))
		{
			Notification->SetCompletionState(SNotificationItem::CS_Fail);
		}
		return;
	}

	HighResScreenshotToolbar::bWaitingFor4KShot = true;
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

bool FHighResScreenshotToolbarModule::CanTake4KScreenshot() const
{
	return GCurrentLevelEditingViewportClient != nullptr && !HighResScreenshotToolbar::bWaitingFor4KShot;
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
