# Compatibility Verification

Verification date: 2026-08-01

| Engine installation | Platform | Result | Notes |
| --- | --- | --- | --- |
| Unreal Engine 5.8.1 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed; v1.0.1 functional Editor test passed |
| Unreal Engine 5.7.4 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed |
| Unreal Engine 5.5.4 | Win64 | BLOCKED | v1.1.0 retry: local UBT still fails before compiling the plugin |
| Unreal Engine 5.0.3 | Win64 | PASS | v1.1.0 configurable-resolution BuildPlugin passed |

## Unreal Engine 5.5.4 blocker

The installed UE 5.5.4 UnrealBuildTool throws `ArgumentNullException` from
`ModuleRules.IsValidForTarget` before it compiles the plugin module. Rebuilding
the rules assembly and selecting an installed Visual Studio 2022 toolchain did
not move the failure into plugin code. The v1.1.0 retry produced the same failure
before any configurable-resolution source file was compiled.

Do not advertise UE 5.5 support until the engine/toolchain installation can
complete a clean independent plugin build.

## Supported versions for the first Fab submission

- Unreal Engine 5.0
- Unreal Engine 5.7
- Unreal Engine 5.8
- Platform: Win64

Each engine version must use its own independently built plugin ZIP.
