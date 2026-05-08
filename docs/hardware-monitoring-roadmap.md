# Hardware Monitoring Roadmap

This project says "hardware monitor", but that does not mean every sensor is
implemented already. Right now KRNL has the foundation: shared contracts, a
basic app wrapper, a minimal driver, driver status, and a CPUID IOCTL.

Real hardware telemetry is still future work.

## What Hardware Monitoring Means Here

For this project, hardware monitoring can eventually mean:

- CPU identity and feature info
- CPU vendor/model/family style details
- temperature readings
- clock speeds
- sensor readings
- basic platform/device info
- careful hypervisor-visible CPU details
- future telemetry that is useful for research without crossing into unsafe
  kernel tricks

The important part: KRNL should explain what it is reading, where it is reading
it from, and whether that data came from user mode or kernel mode.

## What Exists Today

Currently implemented:

- app/driver communication foundation
- driver status IOCTL
- basic CPUID IOCTL
- app-side display of the driver version and CPU vendor string

Not fully implemented yet:

- real temperature monitoring
- clocks/frequency reporting
- motherboard sensor support
- GPU telemetry
- fan speeds
- voltage readings
- polished UI for live monitoring
- deeper hypervisor trace detection

## CPU Info

CPU info is the safest starting point.

Some CPU information can be queried from user mode. CPUID is one example. The
current driver CPUID path mostly proves that IOCTL communication works; it is not
proof that CPUID must always live in the kernel.

Future CPU info work should decide:

- can this be queried safely from user mode?
- does the app only need to format and display it?
- does it need a shared request/response struct?
- is there a real reason to involve the driver?

## Temperatures

Temperature monitoring is not implemented yet.

Future temperature work should start with documented, boring options first:

- vendor tools or documented APIs when available
- Windows-supported interfaces where practical
- user-mode libraries if they are safe, maintained, and acceptable for the repo

Kernel-mode temperature reads should only be added after a design note explains:

- what hardware/interface is being queried
- why user mode is not enough
- what the failure modes are
- how malformed input is validated
- how this will be tested in a VM first

## Clocks

Clock/frequency reporting is also future work.

Possible future sources may include:

- normal Windows APIs
- performance counters
- CPUID-derived static information
- carefully documented driver-side queries only if needed

Dynamic clock readings can be confusing because modern CPUs boost, sleep, park
cores, and change frequency constantly. The UI should make it clear what a value
means instead of pretending one number tells the whole story.

## Sensors

"Sensors" is a big word. It can mean CPU package temperature, motherboard
chipset sensors, fan speed, voltage rails, GPU telemetry, storage health, and a
lot more.

Future sensor support should be added in small pieces. Each sensor family should
have its own documented source and safety notes.

Do not add random port I/O, undocumented chipset poking, or risky probing just
because a forum post says it works on one machine.

## What Can Be Done In User Mode

Prefer user mode when possible.

Good user-mode candidates:

- formatting and UI
- polling schedule and display refresh
- parsing safe OS-provided data
- CPUID for basic CPU info
- logging
- configuration
- calling documented Windows APIs
- talking to the driver through `DriverClient`

User mode is easier to debug, less risky, and friendlier for beginners.

## What May Require Kernel Mode

Kernel mode may be needed when:

- a documented WDK path requires a driver
- the data truly cannot be accessed safely from user mode
- the project has a clear request/response contract
- validation and failure behavior are designed first
- VM-only testing exists

Driver-side telemetry belongs in `src/driver`. It should be small, explicit, and
boring. Every new IOCTL should validate input and output sizes.

## Where Future Code Should Live

- `src/app`: formatting, CLI/GUI display, polling UI, beginner-friendly messages,
  and calls into `DriverClient`.
- `src/shared`: fixed-size IOCTL constants, enums, request structs, and response
  structs used by both the app and driver.
- `src/driver`: kernel-side query handlers only when user mode is not enough and
  the behavior has been designed safely.
- `docs`: design notes explaining what data is being read, why, and how it is
  tested.
- `tests`: shared contract layout checks and future VM-only integration tests.

## Not Part Of The Roadmap

Do not add:

- driver hiding
- kernel patching
- memory scanning
- anti-cheat bypasses
- stealth/evasion behavior
- undocumented hardware access tricks without written design approval

This can be a hardware monitor and research project without turning into a mess.
