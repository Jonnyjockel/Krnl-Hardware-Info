# Driver

Windows kernel driver code belongs here.

Put WDK project files, driver entry points, device and IOCTL dispatch code, kernel telemetry collection, hardware probing, and hypervisor trace detection logic in this folder.

Current foundation files:

- `KrnlDriver.cpp`: minimal WDM-style driver entry, unload, device creation, symbolic link creation, dispatch routines, and two IOCTL handlers.
- `TODO.md`: explicit list of future driver work and areas that should not be implemented casually.

The current driver only reports basic driver status and can execute a plain CPUID request. Advanced detection, memory scanning, driver hiding, and evasion behavior are intentionally not implemented.

## Future Hardware Monitoring Notes

Driver-side hardware telemetry belongs here only when user mode is not enough.

Before adding a kernel query:

- write down what hardware or Windows interface is being queried
- check whether user mode can do it safely instead
- add/update shared request and response structs
- validate every input and output buffer
- keep the IOCTL small and explicit
- test in a disposable VM

Do not add undocumented probing tricks just to make a sensor number appear.
