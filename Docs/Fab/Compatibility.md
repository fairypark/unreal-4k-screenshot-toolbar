# Compatibility Verification

Verification date: 2026-08-01

| Engine installation | Platform | Result | Notes |
| --- | --- | --- | --- |
| Unreal Engine 5.8.1 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed; v1.0.1 functional Editor test passed |
| Unreal Engine 5.7.4 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed |
| Unreal Engine 5.5.4 | Win64 | PASS | Stock UBT compile/link passed with unrelated broken VisualStudioTools plugin excluded; Editor mounted the result |
| Unreal Engine 5.0.3 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed |

## Unreal Engine 5.5.4 verification note

The installed UE 5.5.4 UnrealBuildTool throws `ArgumentNullException` from
`ModuleRules.IsValidForTarget` because the locally installed
`VisualStudioTools` engine plugin has no registered `ModuleRules` type in the
installed rules assembly. The exception is unrelated to
`HighResScreenshotToolbar`.

Using the stock UE 5.5.4 UnrealBuildTool with only `VisualStudioTools` excluded
completed UHT, all plugin C++ compilation actions, static-library linking, DLL
linking, and metadata generation. A clean Editor smoke project then mounted the
project plugin and loaded its configuration. The first smoke project also
loaded the plugin DLL before crashing in the unrelated installed Fab plugin;
disabling Fab in the clean smoke project removed that unrelated crash.

## Supported versions for the first Fab submission

- Unreal Engine 5.0
- Unreal Engine 5.5
- Unreal Engine 5.7
- Unreal Engine 5.8
- Platform: Win64

Each engine version must use its own independently built plugin ZIP.
