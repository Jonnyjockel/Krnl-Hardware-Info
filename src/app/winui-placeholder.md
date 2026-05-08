# WinUI 3 Placeholder

The current app project is a plain console C++ `vcxproj`. It is not a WinUI 3 /
Windows App SDK project yet, so this folder does not contain buildable XAML.

This placeholder documents the first boring UI shape to build once the project
has a real WinUI 3 setup.

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
