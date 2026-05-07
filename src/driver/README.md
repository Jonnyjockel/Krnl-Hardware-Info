# Driver

Windows kernel driver code belongs here.

Put WDK project files, driver entry points, device and IOCTL dispatch code, kernel telemetry collection, hardware probing, and hypervisor trace detection logic in this folder.

Current foundation files:

- `KrnlDriver.cpp`: minimal WDM-style driver entry, unload, device creation, symbolic link creation, dispatch routines, and two IOCTL handlers.
- `TODO.md`: explicit list of future driver work and areas that should not be implemented casually.

The current driver only reports basic driver status and can execute a plain CPUID request. Advanced detection, memory scanning, driver hiding, and evasion behavior are intentionally not implemented.
