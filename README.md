# 4K Screenshot Toolbar for Unreal Engine

[한국어 소개](README.ko.md)

A small, source-available Unreal Editor plugin that captures the active Level
Editor viewport as an exact **3840 × 2160 PNG**. Use the **4K Shot** toolbar
button or press **F12**.

## Features

- One-click 4K capture from the active Level Editor viewport
- F12 default shortcut, editable in Unreal Editor's Keyboard Shortcuts
- Temporarily enables Game View to hide editor icons and transform widgets
- Restores the viewport state automatically after capture
- Uses Unreal Engine's built-in screenshot completion notification
- Silent operation with no custom success sound or duplicate success toast
- Editor-only module; it is not included in packaged games

Screenshots are written to the project's platform-specific screenshot folder.
On Windows Editor, the default location is:

```text
<YourProject>/Saved/Screenshots/WindowsEditor/
```

## Compatibility

- Tested with Unreal Engine 5.8 on Windows
- Other Unreal Engine 5.x versions have not yet been verified

## Installation

1. Download `HighResScreenshotToolbar-v1.0.0-UE5.8-Win64.zip` from the
   [latest release](https://github.com/fairypark/unreal-4k-screenshot-toolbar/releases/latest).
2. Extract `HighResScreenshotToolbar` into your project's `Plugins` directory:

   ```text
   <YourProject>/Plugins/HighResScreenshotToolbar/
   ```

3. Open the project.
4. Enable **4K Screenshot Toolbar** in `Edit > Plugins` if it is not already enabled.
5. Restart the editor when requested.

The ready-to-install package is built for Unreal Engine 5.8 on Win64 and also
includes the source. Users of another engine version should clone the repository
or download GitHub's source archive and build it with a compatible C++ toolchain.

## Usage

Activate the Level Editor viewport, then either:

- click **4K Shot** on the Level Editor toolbar, or
- press **F12**.

To change the shortcut, open `Edit > Keyboard Shortcuts` (`Ctrl+Alt+K`), search
for `4K Shot`, and assign a different key.

## Notes

- The plugin captures only the active Level Editor viewport.
- A capture cannot start while another plugin capture is still being processed.
- If Game View was disabled, the plugin enables it only for the capture and
  restores the previous viewport state afterward.

## Building from source

Clone or copy this repository into your project's `Plugins` directory and
regenerate project files if your setup requires it. Build the Editor target for
your project using your installed Unreal Engine toolchain.

## License

Released under the [MIT License](LICENSE).

Unreal Engine is a trademark or registered trademark of Epic Games, Inc. This
project is not affiliated with or endorsed by Epic Games.
