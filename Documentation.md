# 4K Screenshot Toolbar Documentation

## Overview

4K Screenshot Toolbar is an Editor-only Unreal Engine code plugin. It captures
the active Level Editor viewport as an exact 3840 × 2160 PNG through either the
**4K Shot** toolbar button or the configurable **F12** shortcut.

The plugin temporarily enables Game View when needed so editor icons and
transform widgets are not visible in the capture. It restores the previous
viewport state after Unreal Engine finishes processing the screenshot.

## Requirements

- Windows
- A supported Unreal Engine version listed below
- A Level Editor viewport

| Unreal Engine | Verification |
| --- | --- |
| 5.8.1 | Independent plugin build and functional Editor test |
| 5.7.4 | Independent plugin build |
| 5.0.3 | Independent plugin build |

Unreal Engine 5.5.4 is not advertised as supported because the available local
installation fails inside UnrealBuildTool before plugin compilation. This is an
engine/toolchain environment failure rather than a confirmed plugin source
failure.

## Installation

### Fab / Epic Games Launcher

After acquiring the plugin from Fab:

1. Open Epic Games Launcher.
2. Go to `Library > Fab Library`.
3. Find **4K Screenshot Toolbar**.
4. Select a supported Unreal Engine version and click **Install to Engine**.
5. Start Unreal Editor.
6. Open `Edit > Plugins`, search for **4K Screenshot Toolbar**, and enable it if
   necessary.
7. Restart Unreal Editor when requested.

### Manual project installation

1. Copy the `HighResScreenshotToolbar` directory into:

   ```text
   <YourProject>/Plugins/HighResScreenshotToolbar/
   ```

2. Open the project.
3. Allow Unreal Engine to compile the plugin if prompted.
4. Enable the plugin under `Edit > Plugins` and restart the editor if requested.

Manual source builds require a C++ toolchain supported by the selected Unreal
Engine version.

## Usage

1. Click inside the Level Editor viewport that you want to capture.
2. Use either:

   - the **4K Shot** camera button on the Level Editor toolbar, or
   - the **F12** keyboard shortcut.

3. Wait for Unreal Engine's standard screenshot completion notification.

On Windows Editor, screenshots are written to:

```text
<YourProject>/Saved/Screenshots/WindowsEditor/
```

## Keyboard shortcut

F12 is the default shortcut.

To change it:

1. Open `Edit > Keyboard Shortcuts` or press `Ctrl+Alt+K`.
2. Search for `4K Shot`.
3. Assign the preferred shortcut.

The shortcut is registered in the Level Editor command context. It does not add
an input binding to packaged games.

## Capture behavior

- Output resolution: exactly 3840 × 2160
- Output format: PNG
- Capture target: active Level Editor viewport
- Editor icons and transform widgets: hidden through temporary Game View
- Viewport state: restored after processing
- Success feedback: Unreal Engine's standard screenshot notification
- Plugin success sound: none
- Concurrent capture: disabled while a plugin capture is still processing

## Troubleshooting

### The toolbar button is disabled

Click inside a Level Editor viewport. The command is unavailable when there is
no active Level Editor viewport or while a previous plugin capture is still
processing.

### F12 does not trigger a capture

Open `Edit > Keyboard Shortcuts`, search for `4K Shot`, and confirm the shortcut.
Another editor command or an external overlay may be using F12. Reassign the
command if necessary.

### The plugin does not appear

Confirm that the plugin is installed in either the project `Plugins` directory
or the Unreal Engine `Engine/Plugins/Marketplace` directory. Then open
`Edit > Plugins`, enable the plugin, and restart Unreal Editor.

### A source build fails

Confirm that the installed Visual Studio and MSVC components match the
requirements of the Unreal Engine version. Precompiled Fab packages do not
require the end user to compile the plugin.

## Uninstallation

1. Disable **4K Screenshot Toolbar** under `Edit > Plugins`.
2. Restart Unreal Editor.
3. Remove the plugin through Epic Games Launcher, or delete the manually
   installed `HighResScreenshotToolbar` directory while Unreal Editor is closed.

## Privacy and network access

The plugin performs no network requests, analytics, telemetry, account access,
or background uploads. Screenshots remain in the local project screenshot
directory.

## Support and source

- Documentation and source:
  https://github.com/fairypark/unreal-4k-screenshot-toolbar
- Issues and support:
  https://github.com/fairypark/unreal-4k-screenshot-toolbar/issues

## License

Copyright (c) 2026 fairypark.

The source repository and the included source code are available under the MIT
License. The Fab listing should clearly disclose this existing open-source
availability.
