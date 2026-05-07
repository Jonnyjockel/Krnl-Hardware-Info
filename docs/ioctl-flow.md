# IOCTL Communication Flow

The app and driver communicate through a named device exposed by the driver.

## Names

- Kernel device name: `KRNL_DEVICE_NAME`
- DOS symbolic link: `KRNL_DOS_DEVICE_NAME`
- User-mode path: `KRNL_USER_DEVICE_PATH`

All names are defined in `src/shared/krnl_contracts.h`.

## Flow

1. The driver creates the kernel device and symbolic link during `DriverEntry`.
2. The app opens `KRNL_USER_DEVICE_PATH` with `CreateFileW`.
3. The app sends requests with `DeviceIoControl`.
4. The driver receives requests in `IRP_MJ_DEVICE_CONTROL`.
5. The driver validates buffer sizes, fills the shared response struct, and completes the IRP.

The current foundation uses `METHOD_BUFFERED` IOCTLs. Input and output share the same system buffer on the driver side, so handlers must copy request data before writing response data.

## Current IOCTLs

- `IOCTL_KRNL_GET_DRIVER_STATUS`: returns `KRNL_DRIVER_STATUS_RESPONSE`.
- `IOCTL_KRNL_QUERY_CPUID`: accepts `KRNL_CPUID_REQUEST` and returns `KRNL_CPUID_RESPONSE`.

TODO: Add tests for every new IOCTL before adding behavior beyond simple hardware information queries.

