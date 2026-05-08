# Implementation Status

## Implemented

- Shared version constants.
- Shared device path constants.
- Shared IOCTL constants.
- Shared request/status enums.
- Shared response structs for driver status, CPUID, and generic status.
- Basic user-mode `DriverClient` wrapper.
- Basic logging helpers.
- Minimal console app entry point.
- Minimal WDM-style driver skeleton.
- Driver status IOCTL handler.
- Basic CPUID IOCTL handler.
- Initial Visual Studio solution and project files.
- Basic MSBuild helper script.
- Compile-time shared contract layout checks.
- Beginner docs for VM-only driver service and test-signing workflows.
- Placeholder clean, test, signing automation, and VM deployment notes.

## Placeholder / TODO

- Driver `.inf` package.
- Driver installation/removal scripts.
- WinUI 3 UI.
- Test-signing automation.
- VM deployment automation.
- Real hardware temperature monitoring.
- Real hypervisor trace research and detection logic.
- Integration tests that install and talk to the driver in a disposable VM.

## Intentionally Not Implemented

- Driver hiding.
- Kernel structure patching.
- Memory scanning.
- Anti-cheat bypass or evasion behavior.
- Undocumented kernel tricks.
