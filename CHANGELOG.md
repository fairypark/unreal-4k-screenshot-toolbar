# Changelog

## 1.2.0 - 2026-11-15

- Copy the captured image to the Windows clipboard after saving its PNG. Paste it into an image-aware app with Ctrl+V.
- Add a Win64 plugin build for Unreal Engine 5.9.0 MegaJam Preview (CL 60351404).

## 1.1.0 - 2026-08-01

- Added a resolution preset selector under Editor Preferences
- Added Full HD, QHD, Ultrawide QHD, 4K, 5K, and 8K presets
- Added custom width and height fields with safe 64–16384 limits
- Kept 4K UHD as the default for existing workflows
- Verified Unreal Engine 5.5.4 compilation, linking, and Editor plugin loading

## 1.0.1 - 2026-07-31

- Added Unreal Engine 5.0 compatibility
- Verified independent Win64 plugin builds with Unreal Engine 5.0.3, 5.7.4, and 5.8.1
- Added offline documentation and Fab submission materials

## 1.0.0 - 2026-07-31

Initial public release.

- Added a **4K Shot** button to the Level Editor toolbar
- Added configurable F12 shortcut support
- Added exact 3840 × 2160 PNG capture
- Added automatic Game View switching and viewport-state restoration
- Reused Unreal Engine's built-in screenshot completion notification
- Kept successful captures silent and free of duplicate notifications
