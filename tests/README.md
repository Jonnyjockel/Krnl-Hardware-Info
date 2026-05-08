# Tests

Tests and validation code belong here.

Use this folder for user-mode unit tests, shared contract tests, driver
interface tests, VM-only integration checks, and test fixtures.

Current foundation files:

- `shared_contract_layout.cpp`: real compile-time checks for shared ABI layout.
- `app_driver_integration_notes.md`: VM-only manual integration checklist for
  later.

## What Is Real Right Now

The real test today is `shared_contract_layout.cpp`.

It does not talk to the driver. It does not need a driver installed. It only
compiles the shared contract header and uses `static_assert` checks to catch ABI
changes that would break app/driver compatibility.

The checks cover:

- exported shared struct sizes
- struct alignment
- field offsets
- fixed shared constant values
- request/status enum values
- IOCTL uniqueness

If someone changes `src/shared/krnl_contracts.h` and this test stops compiling,
that is the point. The app and driver need to agree exactly on this layout.

## Compile-Time Shared Contract Tests

Build this file as a normal C++ test program with the same compiler family used
for the app:

```powershell
cl /std:c++17 /EHsc tests\shared_contract_layout.cpp
```

Or wire it into a future test project once the repo has a proper test runner.

## Future User-Mode App Tests

Planned user-mode tests should stay boring and safe:

- `DriverClient` error handling when the driver is not installed
- formatting and logging behavior
- validation for null response buffers
- helper code that does not require a live kernel driver

These should run on a normal developer machine because they should not install,
load, stop, or remove a driver.

## Future VM-Only Driver Integration Tests

Driver integration tests must run only inside a disposable VM.

Future VM-only checks belong in `app_driver_integration_notes.md` until there is
real automation for them.

Those tests can eventually cover:

- driver service install/load
- opening `\\.\KrnlHardwareInfo`
- `IOCTL_KRNL_GET_DRIVER_STATUS`
- `IOCTL_KRNL_QUERY_CPUID`
- clean stop/remove behavior
- cleanup after failure paths

## Never Test This On A Daily-Use PC

Do not run these on your personal Windows install:

- loading this experimental unsigned driver
- enabling test-signing just for casual testing
- installing/removing the driver service
- driver integration tests
- anything that pokes at kernel behavior outside a disposable VM

The boring compile-time shared contract test is fine. The live driver stuff is
VM-only.
