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
- Hardware-monitoring roadmap documentation.
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
- Full hardware monitoring is not implemented yet. Current telemetry is limited
  to basic driver status and a basic CPUID request path.
- Real hardware temperature monitoring.
- Real clock/frequency monitoring.
- Real motherboard/GPU/fan/voltage sensor support.
- Real hypervisor trace research and detection logic.
- Integration tests that install and talk to the driver in a disposable VM.

## Intentionally Not Implemented

- Driver hiding.
- Kernel structure patching.
- Memory scanning.
- Anti-cheat bypass or evasion behavior.
- Undocumented kernel tricks.

## Hardware Monitoring Status

KRNL is not a complete hardware monitor yet.

What works today is the foundation: the app can talk to the driver, ask for
driver status, and request basic CPUID data. That is useful scaffolding, but it
is not temperature monitoring, sensor polling, fan control, GPU telemetry, or a
finished dashboard.

Future hardware telemetry should follow `docs/hardware-monitoring-roadmap.md`
and stay split across the layers:

- app formatting/UI in `src/app`
- shared ABI structs and IOCTL values in `src/shared`
- driver-side kernel queries in `src/driver` only when truly needed
