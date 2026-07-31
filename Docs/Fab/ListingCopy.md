# Fab Listing Copy

This file contains ready-to-paste listing text for the Fab publisher portal.

## Listing configuration

- Product type: Tools & Plugins
- Suggested subcategory: Engine Tools
- Format: Unreal Engine
- Distribution method: Code Plugin
- License: Fab Standard License
- Price: Free
- Mature content: No
- Generated with AI: Yes
  - Conservative disclosure because AI-assisted development and documentation were used.
- Allows usage with AI: Publisher choice

## Title

4K Screenshot Toolbar

## Short description

Capture the active Unreal Editor Level Viewport at a preset or custom resolution with one toolbar click or the configurable F12 shortcut.

## Long description

4K Screenshot Toolbar adds a dedicated camera button to the Unreal Editor Level Editor toolbar. Choose a standard resolution preset or custom dimensions, then click the button—or press the configurable F12 shortcut—to capture the active Level Editor viewport.

The plugin automatically enters Game View for a clean capture, requests the high-resolution screenshot, restores the previous viewport state, and uses Unreal Editor's existing screenshot notification. It is silent by design and does not add duplicate success popups.

### Features

- One-click high-resolution capture from the active Level Editor viewport
- Full HD, QHD, Ultrawide QHD, 4K, 5K, and 8K presets
- Custom width and height fields from 64 to 16384 pixels
- Configurable editor shortcut, defaulting to F12
- Automatically enables Game View for a clean screenshot
- Restores the previous viewport state after capture
- Uses Unreal Editor's built-in screenshot notification
- No added shutter sound or duplicate success notification
- Editor-only: no runtime or packaged-game dependency
- No network access, telemetry, account, or external service
- Source code and offline documentation included

## Technical information

- Plugin name: HighResScreenshotToolbar
- Module: HighResScreenshotToolbar
- Module type: Editor
- Code modules: 1
- Blueprint assets: None
- Runtime components: None
- Supported platform: Win64
- Supported Unreal Engine versions:
  - Unreal Engine 5.8
  - Unreal Engine 5.7
  - Unreal Engine 5.0
- Source code included: Yes
- Third-party dependencies: None
- Network access: None
- Telemetry: None
- Documentation: https://github.com/fairypark/unreal-4k-screenshot-toolbar/blob/main/Documentation.md
- Support: https://github.com/fairypark/unreal-4k-screenshot-toolbar/issues
- Source: https://github.com/fairypark/unreal-4k-screenshot-toolbar
- License: MIT for the source repository; Fab distribution uses the selected Fab Standard License.

## Suggested tags

- Screenshot
- High Resolution
- 4K
- Editor Tool
- Toolbar
- Level Editor
- Productivity
- Capture
- F12

## Version notes

### Unreal Engine 5.8

Initial Fab release. Verified with Unreal Engine 5.8.1 on Win64 through an independent package build and live editor capture test.

### Unreal Engine 5.7

Initial Fab release. Verified with Unreal Engine 5.7.4 on Win64 through an independent package build.

### Unreal Engine 5.0

Initial Fab release. Verified with Unreal Engine 5.0.3 on Win64 through an independent package build. Includes a compatibility path for the Unreal Engine 5.0 Level Editor viewport API.

## Trademark disclaimer

Unreal Engine is a trademark or registered trademark of Epic Games, Inc. This project is not affiliated with or endorsed by Epic Games, Inc.
