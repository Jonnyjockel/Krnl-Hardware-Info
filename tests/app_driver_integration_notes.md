# VM-Only App / Driver Integration Checklist

This is a future manual checklist, not an automated test yet.

Run it only in a disposable VM after driver install/remove scripts, signing
notes, and deployment docs exist.

> [!WARNING]
> Do not run integration tests on a personal machine.

## Future Manual Checklist

- Start from a clean disposable VM snapshot.
- Confirm test-signing / driver-signing setup is understood before loading
  anything.
- Load the KRNL driver service.
- Confirm the driver service reports a running state.
- Open the device path from user mode:

```text
\\.\KrnlHardwareInfo
```

- Query driver status with `IOCTL_KRNL_GET_DRIVER_STATUS`.
- Confirm the returned version matches `KRNL_PROJECT_VERSION_*`.
- Confirm `driver_loaded` is set.
- Query CPUID with `IOCTL_KRNL_QUERY_CPUID` for leaf `0`.
- Confirm the CPUID response includes a non-empty vendor string.
- Query CPUID with leaf `1`.
- Confirm the response completes and reports the hypervisor-present bit value.
- Send at least one intentionally too-small buffer and confirm the driver fails
  safely.
- Stop the driver service.
- Confirm the driver service reports a stopped state.
- Remove the driver service.
- Confirm the device path no longer opens.
- Confirm generated build/deployment files are cleaned up or documented.
- Revert the VM snapshot when finished.

## Not Wired Up Yet

- No `.inf` package exists yet.
- No install/remove scripts exist yet.
- No automated VM deployment exists yet.
- No automated integration test runner exists yet.

Until those exist, this file is just the map for future VM-only testing.
