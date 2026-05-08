# Shared

Shared contracts between the driver and app belong here.

Put IOCTL definitions, request and response structs, shared enums, version constants, and serialization schemas in this folder. Avoid putting layer-specific behavior here.

Current foundation files:

- `krnl_contracts.h`: version constants, device paths, IOCTL values, request/status enums, and fixed-size request/response structs shared by user mode and kernel mode.

## Future Hardware Monitoring Notes

Future telemetry contracts belong here only when the app and driver both need to
agree on them.

Use this folder for:

- new IOCTL values
- fixed-size request structs
- fixed-size response structs
- shared enums and status values
- reserved fields for future compatibility when useful

Do not put formatting, UI behavior, polling loops, Windows handles, STL
containers, raw pointers, or driver-only implementation details in shared
contracts.
