// Copyright (c) 2026 fairypark. Licensed under the MIT License.

#include "HighResScreenshotToolbar.h"
#include "HighResScreenshotToolbarCommands.h"
#include "HighResScreenshotToolbarSettings.h"
#include "HighResScreenshotToolbarStyle.h"

#include "Editor.h"
#include "Engine/Engine.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HighResScreenshot.h"
#include "ISettingsModule.h"
#include "LevelEditor.h"
#include "LevelEditorViewport.h"
#include "Misc/FileHelper.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Runtime/Launch/Resources/Version.h"
#include "ToolMenus.h"
#include "UnrealClient.h"
#include "Widgets/Notifications/SNotificationList.h"

#if PLATFORM_WINDOWS
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformProcess.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Windows/WindowsHWrapper.h"
#endif

#define LOCTEXT_NAMESPACE "FHighResScreenshotToolbarModule"

namespace HighResScreenshotToolbar
{
	FDelegateHandle ScreenshotProcessedHandle;
	bool bWaitingForScreenshot = false;
	FString PendingScreenshotPath;
	FLevelEditorViewportClient* PendingViewportClient = nullptr;
	bool bChangedGameView = false;
	bool bPreviousShowWidget = true;

#if PLATFORM_WINDOWS
	bool CopyScreenshotToClipboard(const FString& Path)
	{
		const void* Window = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		TArray64<uint8> Png;
		if (!Window || !FFileHelper::LoadFileToArray(Png, *Path))
		{
			return false;
		}

		IImageWrapperModule& Images = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
		TSharedPtr<IImageWrapper> Decoder = Images.CreateImageWrapper(EImageFormat::PNG);
		TArray64<uint8> Pixels;
		if (!Decoder || !Decoder->SetCompressed(Png.GetData(), Png.Num()) || !Decoder->GetRaw(ERGBFormat::BGRA, 8, Pixels))
		{
			return false;
		}
		const int32 Width = Decoder->GetWidth();
		const int32 Height = Decoder->GetHeight();
		const SIZE_T RowBytes = SIZE_T(Width) * sizeof(FColor);
		if (Width <= 0 || Height <= 0 || uint64(Pixels.Num()) != RowBytes * Height)
		{
			return false;
		}
		HGLOBAL Bitmap = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFOHEADER) + RowBytes * Height);
		if (!Bitmap)
		{
			return false;
		}
		auto* Header = static_cast<BITMAPINFOHEADER*>(::GlobalLock(Bitmap));
		if (!Header)
		{
			::GlobalFree(Bitmap);
			return false;
		}

		*Header = {};
		Header->biSize = sizeof(BITMAPINFOHEADER);
		Header->biWidth = Width;
		Header->biHeight = Height;
		Header->biPlanes = 1;
		Header->biBitCount = 32;
		Header->biCompression = BI_RGB;
		// A positive-height DIB stores BGRA rows bottom-up. The PNG decoder supplies them top-down.
		uint8* Dest = reinterpret_cast<uint8*>(Header + 1);
		for (int32 Y = Height - 1; Y >= 0; --Y)
		{
			FMemory::Memcpy(Dest, Pixels.GetData() + Y * RowBytes, RowBytes);
			Dest += RowBytes;
		}
		::GlobalUnlock(Bitmap);

		HWND Owner = static_cast<HWND>(const_cast<void*>(Window));
		for (int32 Attempt = 0; Attempt < 4; ++Attempt)
		{
			if (Attempt > 0)
			{
				FPlatformProcess::SleepNoStats(0.005f);
			}
			if (::OpenClipboard(Owner))
			{
				const bool bPublished = ::EmptyClipboard() && ::SetClipboardData(CF_DIB, Bitmap) != nullptr;
				::CloseClipboard();
				if (bPublished)
				{
					return true; // Windows now owns Bitmap.
				}
				break;
			}
		}
		::GlobalFree(Bitmap);
		return false;
	}
#endif

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

	void HandleScreenshotProcessed()
	{
		if (!bWaitingForScreenshot)
		{
			return;
		}

		bWaitingForScreenshot = false;
		RestoreViewportState();
		const bool bSaved = !PendingScreenshotPath.IsEmpty() && FPaths::FileExists(PendingScreenshotPath);
		if (GetHighResScreenshotConfig().FilenameOverride == PendingScreenshotPath)
		{
			GetHighResScreenshotConfig().FilenameOverride.Reset();
		}

		FText ErrorMessage;
		if (!bSaved)
		{
			ErrorMessage = LOCTEXT("CaptureWriteFailed", "Screenshot failed: the output file was not created.");
		}
#if PLATFORM_WINDOWS
		else if (!CopyScreenshotToClipboard(PendingScreenshotPath))
		{
			ErrorMessage = LOCTEXT("ClipboardCopyFailed", "Screenshot saved, but the image could not be copied to the clipboard.");
		}
#endif
		if (!ErrorMessage.IsEmpty())
		{
			FNotificationInfo Info(ErrorMessage);
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
		if (!ScreenshotProcessedHandle.IsValid())
		{
			ScreenshotProcessedHandle = FScreenshotRequest::OnScreenshotRequestProcessed().AddStatic(&HandleScreenshotProcessed);
		}
	}

	void UnregisterScreenshotDelegates()
	{
		if (ScreenshotProcessedHandle.IsValid())
		{
			FScreenshotRequest::OnScreenshotRequestProcessed().Remove(ScreenshotProcessedHandle);
			ScreenshotProcessedHandle.Reset();
		}

		if (bWaitingForScreenshot && GetHighResScreenshotConfig().FilenameOverride == PendingScreenshotPath)
		{
			GetHighResScreenshotConfig().FilenameOverride.Reset();
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
	if (ViewportClient == nullptr || ViewportClient->Viewport == nullptr)
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

	// Older editors do not broadcast screenshot pixels outside automation. Track the file that this request writes.
	const FString Prefix = FPaths::ConvertRelativePathToFull(GetDefault<UEngine>()->GameScreenshotSaveDirectory.Path / TEXT("HighresScreenshot"));
	FString& Path = HighResScreenshotToolbar::PendingScreenshotPath;
	FFileHelper::GenerateDateTimeBasedBitmapFilename(Prefix, TEXT("png"), Path);
	if (FPaths::FileExists(Path))
	{
		Path = FPaths::GetBaseFilename(Path, false) + TEXT("_") + FGuid::NewGuid().ToString() + TEXT(".png");
	}
	GetHighResScreenshotConfig().FilenameOverride = Path;
	HighResScreenshotToolbar::bWaitingForScreenshot = true;
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
	return GCurrentLevelEditingViewportClient != nullptr && GCurrentLevelEditingViewportClient->Viewport != nullptr
		&& !HighResScreenshotToolbar::bWaitingForScreenshot;
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
