# Architecture

KRNL Hardware Info uses a layered split between kernel-mode collection and user-mode presentation.

## Layers

- `src/driver`: kernel-mode access, hardware telemetry collection, and hypervisor trace detection.
- `src/shared`: the explicit contract between kernel and user mode.
- `src/app`: user-mode control, formatting, UI or CLI, and communication with the driver.

## Support Areas

- `docs`: design notes, safety notes, setup instructions, and research documentation.
- `tests`: validation for shared contracts, user-mode behavior, and driver-facing integration checks.
- `tools`: developer utilities that are useful while building or testing but are not part of the runtime product.
- `third_party`: vendored external code when a dependency must live in the repository.

## Dependency Direction

The intended dependency flow is:

```text
src/app -> src/shared <- src/driver
```

The app and driver may both depend on shared contracts. The shared layer should not depend on either runtime layer.

## Current Foundation

- `src/shared/krnl_contracts.h` defines the ABI between app and driver.
- `src/app/DriverClient.*` wraps `CreateFileW` and `DeviceIoControl`.
- `src/driver/KrnlDriver.cpp` creates the device and handles the initial IOCTLs.

Advanced hypervisor trace detection, hardware monitoring depth, and driver deployment are future work. The current foundation is only the safe project scaffolding needed before those areas are designed.
