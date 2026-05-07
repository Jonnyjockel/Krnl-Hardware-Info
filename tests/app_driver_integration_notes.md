# App Driver Integration Test Notes

TODO: Add VM-only tests after project files and driver installation scripts exist.

Future checks:

- Driver service can be installed and removed in a disposable VM.
- User-mode app opens `KRNL_USER_DEVICE_PATH`.
- `IOCTL_KRNL_GET_DRIVER_STATUS` returns the expected project version.
- `IOCTL_KRNL_QUERY_CPUID` returns register values for a known leaf.
- Invalid buffer sizes fail safely.

Do not run integration tests on a personal machine.

