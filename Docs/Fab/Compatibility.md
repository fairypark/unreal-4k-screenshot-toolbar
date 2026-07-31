# Compatibility Verification

Verification date: 2026-07-31

| Engine installation | Platform | Result | Notes |
| --- | --- | --- | --- |
| Unreal Engine 5.8.1 | Win64 | PASS | Independent BuildPlugin and functional Editor test |
| Unreal Engine 5.7.4 | Win64 | PASS | Independent BuildPlugin |
| Unreal Engine 5.5.4 | Win64 | BLOCKED | Local UBT fails before compiling the plugin |
| Unreal Engine 5.0.3 | Win64 | PASS | Independent BuildPlugin after UE 5.0 source compatibility patch |

## Unreal Engine 5.5.4 blocker

The installed UE 5.5.4 UnrealBuildTool throws `ArgumentNullException` from
`ModuleRules.IsValidForTarget` before it compiles the plugin module. Rebuilding
the rules assembly and selecting an installed Visual Studio 2022 toolchain did
not move the failure into plugin code.

Do not advertise UE 5.5 support until the engine/toolchain installation can
complete a clean independent plugin build.

## Supported versions for the first Fab submission

- Unreal Engine 5.0
- Unreal Engine 5.7
- Unreal Engine 5.8
- Platform: Win64

Each engine version must use its own independently built plugin ZIP.
