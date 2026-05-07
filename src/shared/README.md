# Shared

Shared contracts between the driver and app belong here.

Put IOCTL definitions, request and response structs, shared enums, version constants, and serialization schemas in this folder. Avoid putting layer-specific behavior here.

Current foundation files:

- `krnl_contracts.h`: version constants, device paths, IOCTL values, request/status enums, and fixed-size request/response structs shared by user mode and kernel mode.
