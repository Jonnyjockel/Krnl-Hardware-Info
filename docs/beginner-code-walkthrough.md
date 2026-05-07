# Beginner Code Walkthrough

This document is a practical tour of the current KRNL Hardware Info codebase.
It is written for someone who knows basic C/C++ but is still getting used to
Windows drivers, IOCTLs, CPUID, and app-to-driver communication.

The short version: this repo currently contains a small foundation. It has a
shared contract header, a tiny user-mode console app, a minimal kernel driver,
placeholder build/clean tooling, and a couple of test notes. It is not a full
hardware monitor yet.

## 1. What this project currently is

KRNL Hardware Info is currently a Windows app/driver skeleton.

The current code can describe the basic shape of a kernel-backed hardware info
tool:

```text
user-mode app
    |
    | CreateFileW + DeviceIoControl
    v
kernel driver
    |
    | shared structs and IOCTL IDs
    v
src/shared/krnl_contracts.h
```

Implemented right now:

- A shared header that defines device names, IOCTL values, versions, enums, and
  request/response structs.
- A user-mode `DriverClient` wrapper around `CreateFileW` and
  `DeviceIoControl`.
- A small console `main.cpp` that opens the driver, asks whether it is loaded,
  and tries one CPUID query.
- A minimal WDM-style kernel driver that creates a device, creates a symbolic
  link, handles create/close requests, and handles two IOCTLs.
- A basic CPUID IOCTL that runs `__cpuidex` in the driver and returns the raw
  register values.
- Placeholder build, clean, VM deployment, and integration-test notes.

## 2. What this project is NOT yet

This repo is not a complete hardware monitor yet.

It does not currently include:

- Visual Studio solution/project files.
- A WDK driver project file.
- A WinUI 3 frontend.
- Driver installation/removal scripts.
- Test-signing automation.
- Real temperature, fan, voltage, or sensor telemetry.
- Full hypervisor trace detection.
- VM integration tests that install the driver and talk to it.
- Production-grade packaging or deployment.

It also intentionally does not implement driver hiding, kernel patching, memory
scanning, anti-cheat bypasses, or evasion behavior.

## 3. Folder structure explained

```text
KRNL Hardware Info/
    README.md
    docs/
    src/
        shared/
        app/
        driver/
    tools/
    tests/
```

`README.md`

The root README explains the project's rough purpose and the author's safety
warnings. The big practical takeaway is: test this in a VM, not on your main
machine.

`docs/`

Documentation lives here.

- `architecture.md` explains the intended layer split.
- `ioctl-flow.md` explains the app-to-driver communication path.
- `implementation-status.md` lists what is implemented, placeholder, and
  intentionally not implemented.
- `safety.md` contains safety guidance.
- This file, `beginner-code-walkthrough.md`, explains the current code in a
  beginner-friendly way.

`src/shared/`

Shared app/driver contracts live here. This is the agreement layer. Both the app
and driver include `krnl_contracts.h`.

Current files:

- `krnl_contracts.h`: shared device names, IOCTL numbers, version constants,
  enums, and fixed-size structs.
- `README.md`: short note explaining what belongs in this folder.

`src/app/`

User-mode code lives here. "User mode" means normal Windows application code,
outside the kernel.

Current files:

- `DriverClient.h` / `DriverClient.cpp`: opens the driver device and sends
  IOCTLs.
- `Logger.h` / `Logger.cpp`: simple console and debug-output logging.
- `main.cpp`: tiny console entry point that exercises the driver client.
- `README.md`: short note about the app layer.

`src/driver/`

Kernel-mode driver code lives here. "Kernel mode" means code running inside the
Windows kernel, where bugs can crash the whole OS.

Current files:

- `KrnlDriver.cpp`: minimal driver entry point, unload routine, dispatch
  routines, and IOCTL handlers.
- `README.md`: short note about what belongs in the driver folder.
- `TODO.md`: driver work that still needs design and testing.

`tools/`

Developer helper scripts and notes live here.

Current files:

- `build.ps1`: placeholder build helper. It looks for `.sln`, `.vcxproj`, or
  `.wixproj` files and reports that real project files still need to be added.
- `clean.ps1`: dry-run clean helper. Actual deletion is intentionally disabled
  until build output paths are known.
- `vm-deployment-notes.md`: notes for future VM deployment and test-signing
  helpers.
- `README.md`: short note about the tools folder.

`tests/`

Tests and test notes live here.

Current files:

- `shared_contract_layout.cpp`: compile-time checks for shared struct sizes and
  IOCTL uniqueness.
- `app_driver_integration_notes.md`: VM-only integration test checklist for the
  future.
- `README.md`: short note about the tests folder.

## 4. Shared contracts explained

### What `krnl_contracts.h` is

`src/shared/krnl_contracts.h` is the shared agreement between the user-mode app
and the kernel driver.

It defines:

- Project version constants.
- Device names.
- IOCTL codes.
- Request and status enums.
- Structs passed between app and driver.

Both sides include this same file:

```text
src/app/DriverClient.cpp
        |
        v
src/shared/krnl_contracts.h
        ^
        |
src/driver/KrnlDriver.cpp
```

That matters because app-driver communication is not magical. The app sends a
blob of bytes. The driver reads that blob as a C struct. If the app and driver
disagree about the struct layout, the bytes can be interpreted incorrectly.

### Why shared structs matter

These structs are the messages passed between app and driver.

Current request/response structs include:

- `KRNL_DRIVER_STATUS_RESPONSE`: returned by the driver status IOCTL.
- `KRNL_CPUID_REQUEST`: sent by the app when asking for a CPUID leaf/subleaf.
- `KRNL_CPUID_RESPONSE`: returned by the driver with CPUID register values.
- `KRNL_GENERIC_STATUS_RESPONSE`: defined for generic status messaging, but not
  currently used by the app or driver IOCTL handlers.

The tests in `tests/shared_contract_layout.cpp` use `static_assert` to catch
accidental layout changes at compile time.

### What IOCTLs are

IOCTL means "I/O control code."

In plain English, an IOCTL is a numbered command that an app can send to a
driver. For example:

- `IOCTL_KRNL_GET_DRIVER_STATUS`: "driver, tell me your basic status."
- `IOCTL_KRNL_QUERY_CPUID`: "driver, run this CPUID query and return the raw
  registers."

The IOCTL numbers are defined in `krnl_contracts.h` so the app and driver use
the exact same command IDs.

### Why struct layouts must match

Imagine the app sends this:

```text
KRNL_CPUID_REQUEST
    leaf    = 0
    subleaf = 0
```

The driver receives bytes, not a friendly C++ object. It needs to know that the
first 4 bytes are `leaf` and the next 4 bytes are `subleaf`.

If one side changes the struct and the other side does not, the driver might
read the wrong fields, write too much data, or return data the app cannot
understand. In kernel code, that kind of mismatch can become a crash, not just a
normal app bug.

## 5. User-mode app explained

The user-mode app currently lives in `src/app`.

### What `DriverClient` does

`DriverClient` is a small C++ wrapper around the raw Win32 driver communication
calls.

It owns a Windows `HANDLE` named `device_handle_`.

Its main jobs are:

- Open the driver device.
- Close the driver handle automatically.
- Send IOCTLs.
- Return beginner-readable status info through `DriverClientStatus`.

This keeps `main.cpp` from having to call `CreateFileW` and `DeviceIoControl`
directly.

### How `CreateFileW` talks to a driver

`CreateFileW` is usually used to open files, but Windows also uses it to open
devices.

In `DriverClient::Open`, the app calls:

```cpp
CreateFileW(KRNL_USER_DEVICE_PATH, ...);
```

`KRNL_USER_DEVICE_PATH` is defined as:

```cpp
L"\\\\.\\KrnlHardwareInfo"
```

That user-mode path points to the symbolic link created by the driver. If the
driver is not loaded, or the symbolic link does not exist, opening the handle
fails.

### What `DeviceIoControl` does

`DeviceIoControl` sends an IOCTL request through the handle opened by
`CreateFileW`.

In this project, `DriverClient::SendIoctl` wraps the call:

```text
DeviceIoControl(
    driver handle,
    IOCTL code,
    input buffer,
    input size,
    output buffer,
    output size
)
```

If the call succeeds, the output buffer contains whatever response the driver
wrote.

### What `QueryDriverStatus` does

`DriverClient::QueryDriverStatus` asks the driver for basic status.

It sends:

```text
IOCTL_KRNL_GET_DRIVER_STATUS
```

It expects the driver to fill:

```text
KRNL_DRIVER_STATUS_RESPONSE
```

The current driver fills in:

- Version `0.1.0`.
- `status_code = KrnlStatusOk`.
- `driver_loaded = 1`.
- `test_build = 1`.

This is a simple "is the driver alive and speaking the same contract?" check.

### What `QueryCpuid` does

`DriverClient::QueryCpuid` sends a CPUID request to the driver.

The app provides:

```text
KRNL_CPUID_REQUEST
    leaf
    subleaf
```

The driver returns:

```text
KRNL_CPUID_RESPONSE
    leaf
    subleaf
    eax
    ebx
    ecx
    edx
    hypervisor_present
    vendor_string
```

Right now, the app only asks for leaf `0`, subleaf `0`, which usually returns
the CPU vendor string such as `GenuineIntel` or `AuthenticAMD`.

Important detail: the current app does not ask leaf `1`, so the app does not
currently print the `hypervisor_present` flag.

### What `Logger` does

`Logger` is tiny on purpose.

`LogInfo` writes to:

- Visual Studio/debugger output through `OutputDebugStringA`.
- Standard output through `std::cout`.

`LogError` writes to:

- Visual Studio/debugger output through `OutputDebugStringA`.
- Standard error through `std::cerr`.

There is no log file, log rotation, GUI logging, or structured logging yet.

### What `main.cpp` currently does

`main.cpp` is a minimal console smoke test.

It does this:

```text
create DriverClient
    |
open driver
    |
query driver status
    |
print version and loaded flag
    |
query CPUID leaf 0
    |
print CPU vendor if the query succeeds
```

If opening the driver or querying driver status fails, the app logs an error and
returns `1`.

If the CPUID query fails, the app logs the CPUID error but still returns `0`.
That means CPUID failure is currently treated as non-fatal after driver status
has succeeded.

## 6. Kernel driver explained

The kernel driver currently lives in `src/driver/KrnlDriver.cpp`.

This is WDM-style driver code. WDM stands for Windows Driver Model. It is a
lower-level driver style than writing a normal desktop app.

### What `DriverEntry` is

`DriverEntry` is the driver's starting point. It is the kernel-driver equivalent
of `main`.

Current `DriverEntry` does this:

```text
initialize device name
initialize symbolic link name
create device object
create symbolic link
install dispatch routines
set unload routine
mark device ready
return STATUS_SUCCESS
```

If device creation fails, the driver returns the failure status.

If symbolic link creation fails, the driver deletes the device object before
returning. That cleanup path matters because kernel objects should not be left
behind after a failed startup.

### What the unload routine does

`KrnlDriverUnload` runs when the driver unloads.

It deletes:

- The symbolic link.
- The device object, if it exists.

This is the driver cleanup path. Kernel cleanup paths are important because
leaked kernel objects can create weird behavior until reboot.

### What device objects and symbolic links are

The device object is the kernel-side object:

```text
\Device\KrnlHardwareInfo
```

The symbolic link is the user-visible bridge:

```text
\DosDevices\KrnlHardwareInfo
```

The app opens this path:

```text
\\.\KrnlHardwareInfo
```

The rough mapping is:

```text
app path:       \\.\KrnlHardwareInfo
                    |
DOS link:       \DosDevices\KrnlHardwareInfo
                    |
kernel device:  \Device\KrnlHardwareInfo
```

Common beginner confusion: the app does not open `\Device\KrnlHardwareInfo`
directly. It opens the user-mode path that resolves through the symbolic link.

### What dispatch routines are

Dispatch routines are driver functions that handle different request types.

Current dispatch setup:

- `IRP_MJ_CREATE` -> `KrnlDispatchCreateClose`
- `IRP_MJ_CLOSE` -> `KrnlDispatchCreateClose`
- `IRP_MJ_DEVICE_CONTROL` -> `KrnlDispatchDeviceControl`
- Everything else -> `KrnlDispatchUnsupported`

An IRP is an I/O Request Packet. It is how Windows represents an I/O request
inside the kernel.

### How the IOCTL switch works

When the app calls `DeviceIoControl`, the driver receives an
`IRP_MJ_DEVICE_CONTROL` request.

`KrnlDispatchDeviceControl` extracts:

- The IOCTL code.
- Input buffer length.
- Output buffer length.
- The system buffer pointer.

Then it switches on the IOCTL code:

```text
IOCTL_KRNL_GET_DRIVER_STATUS
    -> HandleGetDriverStatus

IOCTL_KRNL_QUERY_CPUID
    -> HandleQueryCpuid

anything else
    -> STATUS_INVALID_DEVICE_REQUEST
```

Every path completes the IRP through `CompleteIrp`.

### What `METHOD_BUFFERED` means

The current IOCTLs use `METHOD_BUFFERED`.

Beginner version: Windows gives the driver one kernel-managed buffer for the
request and response. The driver reads input from that buffer and writes output
back to that same buffer.

That is why `HandleQueryCpuid` copies the request first:

```text
copy request out of system buffer
zero response area
write response into system buffer
```

If the driver wrote the response before saving the request, it could overwrite
the input values it still needs.

### What the status IOCTL does

`HandleGetDriverStatus` validates that the output buffer is large enough for
`KRNL_DRIVER_STATUS_RESPONSE`.

If the buffer is valid, it zeroes the response and fills:

- Version constants from `krnl_contracts.h`.
- `KrnlStatusOk`.
- `driver_loaded = 1`.
- `test_build = 1`.

Then it reports that it wrote the size of `KRNL_DRIVER_STATUS_RESPONSE`.

### What the CPUID IOCTL does or is supposed to do

`HandleQueryCpuid` currently:

1. Checks that the input buffer is large enough for `KRNL_CPUID_REQUEST`.
2. Checks that the output buffer is large enough for `KRNL_CPUID_RESPONSE`.
3. Copies the request because `METHOD_BUFFERED` reuses the same buffer.
4. Calls `__cpuidex`.
5. Copies the returned register values into the response.
6. If the requested leaf is `0`, fills `vendor_string`.
7. If the requested leaf is `1`, sets `hypervisor_present` from ECX bit 31.

What it does not do yet:

- It does not parse lots of CPUID leaves into friendly names.
- It does not perform deep hypervisor trace analysis.
- It does not combine CPUID with other hardware telemetry.
- It does not expose a UI for browsing CPUID data.

## 7. IOCTL flow explained step by step

Example: the app calls `DriverClient::QueryCpuid`.

```text
1. main.cpp creates a KRNL_CPUID_REQUEST
       leaf = 0
       subleaf = 0

2. main.cpp calls DriverClient::QueryCpuid

3. DriverClient checks that the response pointer is not null

4. DriverClient::SendIoctl calls DeviceIoControl
       IOCTL_KRNL_QUERY_CPUID
       input  = KRNL_CPUID_REQUEST
       output = KRNL_CPUID_RESPONSE

5. Windows sends an IRP_MJ_DEVICE_CONTROL request to the driver

6. KrnlDispatchDeviceControl reads the IOCTL code and buffer sizes

7. The switch matches IOCTL_KRNL_QUERY_CPUID

8. HandleQueryCpuid validates the input and output buffer sizes

9. HandleQueryCpuid copies the request out of the shared METHOD_BUFFERED buffer

10. HandleQueryCpuid calls __cpuidex

11. HandleQueryCpuid fills KRNL_CPUID_RESPONSE

12. The driver completes the IRP

13. DeviceIoControl returns to the app

14. main.cpp prints the CPU vendor string if the call succeeded
```

Same idea, shorter diagram:

```text
main.cpp
  -> DriverClient::QueryCpuid
    -> DeviceIoControl(IOCTL_KRNL_QUERY_CPUID)
      -> KrnlDispatchDeviceControl
        -> HandleQueryCpuid
          -> __cpuidex
        <- KRNL_CPUID_RESPONSE
    <- response buffer
  -> LogInfo("CPU vendor: ...")
```

## 8. CPUID explained simply

### What CPUID is

CPUID is a CPU instruction. Software can call it to ask the processor questions
about itself.

Examples of things CPUID can report:

- CPU vendor string.
- Supported CPU features.
- Processor family/model information.
- Whether a hypervisor bit is set.
- Hypervisor vendor leaves on some systems.

CPUID uses numbered queries called leaves. Some leaves also use a subleaf.

### Why it can be called from user-mode and kernel-mode

CPUID is not a privileged instruction on normal x86/x64 systems. That means
normal apps can call it, and kernel drivers can call it too.

So why call it from the driver here?

For this project, the driver path is the foundation for a future kernel-backed
hardware monitor. Calling CPUID through the driver proves the app/driver IOCTL
path works with a simple hardware-related query.

It does not mean CPUID requires kernel mode.

### What EAX, EBX, ECX, and EDX are

`EAX`, `EBX`, `ECX`, and `EDX` are CPU registers. A register is a tiny storage
slot inside the CPU.

With CPUID:

- The caller puts the requested leaf in `EAX`.
- The caller often puts the requested subleaf in `ECX`.
- The CPU returns answers in `EAX`, `EBX`, `ECX`, and `EDX`.

The project returns those raw values in `KRNL_CPUID_RESPONSE`.

### Why hypervisors may show up through CPUID

A hypervisor is software that runs virtual machines. Some hypervisors announce
themselves through CPUID.

One common signal is CPUID leaf `1`, ECX bit `31`. If that bit is set, software
usually treats that as "a hypervisor is present."

Important: this is only one signal. A serious compatibility or research tool
would need careful parsing and documentation. The current project does not
implement deep detection logic.

### What this project currently checks

Currently:

- Leaf `0` returns the CPU vendor string.
- Leaf `1` sets `hypervisor_present` if ECX bit 31 is set.
- The app currently requests only leaf `0`, so it prints only the vendor string.

### What is still TODO

Still TODO:

- Request leaf `1` from the app if the app wants to display the hypervisor bit.
- Parse more CPUID leaves safely and clearly.
- Document what each parsed field means.
- Add tests for CPUID parsing.
- Keep the feature focused on hardware information and research, not hiding or
  bypass behavior.

## 9. Safety notes

Test this in a VM.

Kernel bugs are not like normal app bugs. A normal app bug might crash the app.
A kernel bug can crash Windows with a BSOD.

Unsigned or test-signed drivers are also risky on a personal machine:

- They may require changing boot/test-signing settings.
- Anti-cheat products may complain about unsigned kernel drivers.
- A vulnerable driver can become a serious local security problem.
- Bad cleanup or bad pointer handling can destabilize the OS.

This current code is not malware, not a cheat, and not an evasion tool. It does
not contain persistence, hiding, credential theft, process injection,
anti-cheat bypass logic, kernel patching, or memory scanning.

It is still kernel code, though, so treat it with respect. Use a disposable VM.

## 10. How to modify the project safely

### Where to add new IOCTLs

Start in `src/shared/krnl_contracts.h`.

Add:

- A new function number.
- A new `IOCTL_KRNL_*` define.
- Any request/response structs needed by that IOCTL.
- Compile-time layout checks if the struct crosses the app/driver boundary.

Then update:

- `src/driver/KrnlDriver.cpp` to handle the new IOCTL.
- `src/app/DriverClient.*` to expose a clean user-mode method.
- `tests/shared_contract_layout.cpp` to verify layout and uniqueness.

### Where to add new shared structs

Add shared structs in `src/shared/krnl_contracts.h` only when both app and
driver need the same layout.

Keep them boring:

- Fixed-size integer types like `uint32_t`.
- Fixed-size arrays instead of dynamic strings.
- No pointers.
- No C++ types like `std::string` or `std::vector`.

Pointers and C++ containers do not safely cross the user/kernel boundary.

### Where to add driver logic

Add driver-side behavior in `src/driver`.

For this codebase right now, that means adding helper functions near the current
IOCTL handlers in `KrnlDriver.cpp`, or splitting into new driver files once the
WDK project structure exists.

Every driver IOCTL handler should:

- Validate input buffer size.
- Validate output buffer size.
- Copy input before writing output when using `METHOD_BUFFERED`.
- Zero output structs before filling them.
- Return a clear `NTSTATUS`.
- Complete the IRP exactly once.

### Where to add UI/app logic

Add user-mode behavior in `src/app`.

For now:

- Add driver-call wrappers to `DriverClient`.
- Add console display logic to `main.cpp`.
- Add logging through `Logger`.

Later, when WinUI 3 exists, UI pages/viewmodels should call `DriverClient`
instead of calling `DeviceIoControl` directly.

### How to avoid breaking app-driver compatibility

The safest habit is to treat `krnl_contracts.h` like a public API.

Before changing a shared struct, ask:

- Will old app code still understand the new driver response?
- Will old driver code still understand the new app request?
- Did the struct size change?
- Did field order change?
- Did alignment or padding change?
- Did the tests catch the change?

If compatibility matters, prefer adding a new struct or versioned IOCTL instead
of silently changing an existing layout.

## 11. Common beginner mistakes

Changing structs in the app but not the driver.

The app and driver must agree on shared structs. If one side changes
`KRNL_CPUID_RESPONSE`, the other side needs to be updated too.

Forgetting buffer size checks.

Never assume the app sent a correctly sized buffer. Even your own app can have a
bug. The driver must check sizes every time.

Assuming kernel code is like normal C++.

Kernel code has stricter rules. You cannot casually use every user-mode API,
allocate memory the same way, sleep/block wherever you want, or recover from
bad pointer mistakes as easily.

Forgetting cleanup paths.

If `IoCreateSymbolicLink` fails after `IoCreateDevice` succeeds, the device
object still needs to be deleted. The current driver handles that case.

Confusing symbolic link name with device name.

The kernel device name is `\Device\KrnlHardwareInfo`.

The DOS symbolic link is `\DosDevices\KrnlHardwareInfo`.

The app opens `\\.\KrnlHardwareInfo`.

These are related, but they are not the same string.

Forgetting that `METHOD_BUFFERED` reuses one buffer.

For buffered IOCTLs, input and output share the same system buffer on the driver
side. Copy the request before writing the response.

Treating CPUID as full hypervisor detection.

CPUID can expose useful signals, but one bit or one vendor string is not a full
detection engine. The current code is basic CPUID plumbing.

Not testing in a VM.

Driver development belongs in a disposable VM. A mistake can BSOD the machine.

Adding exciting driver tricks without a design.

Do not casually add hiding, kernel patching, memory scanning, or undocumented
kernel behavior. This repo currently calls those out as intentionally not
implemented.

## 12. Current TODO roadmap

The current project is ready for foundation work, not advanced telemetry yet.

Near-term missing pieces:

- Visual Studio solution and project files.
- WDK project file for the driver.
- Proper build script wiring once project files exist.
- Driver install/remove scripts for a disposable VM.
- Test-signing workflow.
- VM-only integration tests.

App/UI missing pieces:

- WinUI 3 frontend.
- User-friendly display models.
- UI flow for driver status, CPUID, and future hardware telemetry.
- Clear error display when the driver is not loaded.

Driver/hardware missing pieces:

- Real hardware telemetry.
- Safer and broader CPUID parsing.
- Documented kernel APIs for hardware information where available.
- Tests for invalid buffers and unsupported IOCTLs.
- A written design before adding deeper hypervisor trace research.

Tooling missing pieces:

- `tools/build.ps1` needs to call MSBuild after `.sln`/`.vcxproj` files exist.
- `tools/clean.ps1` needs finalized output paths before deletion is enabled.
- `tools/vm-deployment-notes.md` needs exact commands after the driver service
  name, WDK project, and VM deployment target are finalized.

## Quick mental model

If you remember only one picture, remember this:

```text
main.cpp
  uses DriverClient
    opens \\.\KrnlHardwareInfo
      reaches \DosDevices\KrnlHardwareInfo
        points to \Device\KrnlHardwareInfo
          handled by KrnlDriver.cpp
            uses structs from krnl_contracts.h
```

The shared header is the handshake. The app sends IOCTLs. The driver validates
buffers, fills shared response structs, and completes requests. Everything else
in the repo is currently scaffolding around that core idea.
