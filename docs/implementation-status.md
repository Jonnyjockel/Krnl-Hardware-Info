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
- Hypervisor detection roadmap documentation.
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
- CPUID hypervisor vendor leaf `0x40000000` display/parsing.
- SMBIOS/BIOS virtualization artifact checks.
- PCI/device virtualization artifact checks.
- Timing checks, if they ever happen, are future research only.
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

## Hypervisor Detection Status

KRNL does not have a full hypervisor detection engine yet.

Currently implemented:

- driver status IOCTL
- generic CPUID IOCTL request/response path
- CPUID leaf `0` vendor-string handling in the response
- CPUID leaf `1` hypervisor-present bit handling in the response

The current console app only requests CPUID leaf `0`, so the visible app output
is still just the CPU vendor string plus driver status. Leaf `1` support exists
in the driver response path, but the app does not display it yet.

Planned safe research checks:

- CPUID hypervisor-present bit display
- CPUID hypervisor vendor leaf `0x40000000`
- SMBIOS/BIOS strings from documented user-mode sources where possible
- PCI/device artifacts from documented enumeration paths
- timing checks only as future, VM-only research with a written design

Advanced detection is not implemented:

- no stealth checks
- no bypass logic
- no anti-cheat evasion
- no driver hiding
- no vulnerable-driver loading
- no undocumented kernel tricks

The project framing is learning, virtualization artifact research, hardware
telemetry, and VM safety checks.
