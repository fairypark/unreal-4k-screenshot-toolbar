# 4K Screenshot Toolbar v1.1.0

This release adds a hybrid preset and custom-resolution workflow while keeping
4K UHD as the default.

## What's new

- Added resolution settings under
  `Edit > Editor Preferences > Plugins > 4K Screenshot Toolbar`
- Added Full HD, QHD, Ultrawide QHD, 4K UHD, 5K, and 8K UHD presets
- Added a `Custom` option with width and height fields
- Limited each custom dimension to 64–16384 pixels
- Applied the same saved resolution to both the toolbar button and F12 shortcut

## Compatibility

- Unreal Engine 5.8.1 / Win64: independent plugin build passed
- Unreal Engine 5.7.4 / Win64: independent plugin build passed
- Unreal Engine 5.0.3 / Win64: independent plugin build passed

Unreal Engine 5.5.4 remains unadvertised because the available local engine
installation fails inside UnrealBuildTool before compiling the plugin module.
