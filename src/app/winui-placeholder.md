# WinUI 3 Placeholder

The current app project is a plain console C++ `vcxproj`. It is not a WinUI 3 /
Windows App SDK project yet, so this folder does not contain buildable XAML.

This placeholder documents the first boring UI shape to build once the project
has a real UI setup.

## Tech Decision (OPEN — needs Jonny's call)

The GUI framework is NOT decided yet. This is a deliberate open question, not a
silent assumption. Options being weighed:

- **WinUI 3 (C++/WinRT)** — natural continuation of the existing C++ codebase;
  modern Windows look; packaged/unpackaged complexity; heavier project setup.
- **C# / WPF** — very fast UI development, huge ecosystem, easy MVVM; introduces
  a second language + an app/driver boundary across languages (C# app calling a
  C++ contract is fine, but it is a bigger architectural shift).
- **C++ / Qt** — cross-platform, mature widgets; adds a third-party framework
  dependency and its own build tooling.
- **Rust / egui or Tauri** — modern, lightweight; a large jump from the current
  C++ foundation and adds Rust tooling to the repo.
- **Web UI + local bridge** — any frontend calling a local service; flexible but
  adds a process boundary and more moving parts.

Current default leaning: **WinUI 3**, because it stays in C++ and keeps the
`app -> shared <- driver` layer split clean. But no code is written until Jonny
confirms. See the OPEN question in `Stuffs/TODO.md`.

## Missing Project Pieces

- Windows App SDK package/reference.
- WinUI 3 project type or packaging setup.
- `App.xaml`, `MainWindow.xaml`, and generated XAML build targets.
- A decision on packaged vs unpackaged app deployment.

Until those exist, adding XAML files would look nice in the repo but would not
actually build. That is not helpful for beginners.

## Minimal UI Skeleton

First screen:

```text
KRNL Hardware Info

Driver connection: Not connected / Connected / Failed

[Query Driver Status] [Query CPUID]

Driver status:
  Version:
  Loaded:
  Test build:

CPU:
  Vendor:
  Hypervisor present:

CPUID registers:
  EAX:
  EBX:
  ECX:
  EDX:

Output:
  log lines and beginner-friendly errors
```

## Architecture Rule

WinUI code should not call `CreateFileW`, `DeviceIoControl`, or any raw driver
API directly.

The UI should call an app-layer object such as `HardwareInfoService`, which then
uses `DriverClient`.

```text
WinUI page/viewmodel
    -> HardwareInfoService
        -> DriverClient
            -> DeviceIoControl
```

## Button Behavior

`Query Driver Status` should:

- connect to the driver if needed
- call `HardwareInfoService::QueryDriverStatus`
- update driver connection/status fields
- append a readable log line

`Query CPUID` should:

- connect to the driver if needed
- call `HardwareInfoService::QueryCpuVendor`
- call `HardwareInfoService::QueryHypervisorPresentBit`
- update CPU vendor and hypervisor-present fields
- show EAX/EBX/ECX/EDX with the short beginner explanation from `CpuidText`

This is still basic CPUID display, not advanced hypervisor detection.
