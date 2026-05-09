# Implementation Status

## Implemented

- Shared version constants.
- Shared device path constants.
- Shared IOCTL constants.
- Shared request/status enums.
- Shared response structs for driver status, CPUID, and generic status.
- Basic user-mode `DriverClient` wrapper.
- Basic user-mode `HardwareInfoService` wrapper.
- Beginner-friendly CPUID console formatting.
- Basic logging helpers.
- Minimal console app entry point.
- User-mode app builds/runs as a smoke test when the local MSVC/SDK setup is
  available.
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
- WinUI 3 UI. A placeholder layout exists, but no Windows App SDK project is
  wired up yet.
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

## User-Mode App Runtime Status

The user-mode app is meant to run as a smoke test first.

Current expected behavior:

- If the KRNL driver is not built, installed, and started, the app should launch
  and then report that `\\.\KrnlHardwareInfo` was not found.
- Win32 error 2 / `ERROR_FILE_NOT_FOUND` is expected in that state because the
  driver device symbolic link does not exist yet.
- To get past that point, the driver needs the WDK/test-signing/VM workflow
  described in the build and driver-service docs.

This is not a hardware telemetry failure. It is the app correctly saying the
driver side is not loaded yet.

## Hypervisor Detection Status

KRNL does not have a full hypervisor detection engine yet.

Currently implemented:

- driver status IOCTL
- generic CPUID IOCTL request/response path
- CPUID leaf `0` vendor-string handling in the response
- CPUID leaf `1` hypervisor-present bit handling in the response

The current console app requests CPUID leaf `0` for the CPU vendor string and
leaf `1` for the hypervisor-present bit. It prints the bit as one signal, not as
a perfect detector.

Planned safe research checks:

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
