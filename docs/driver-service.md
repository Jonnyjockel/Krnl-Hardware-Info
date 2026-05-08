# Driver Service Notes

This page is for manually installing, starting, stopping, and removing the KRNL
driver during development.

> [!WARNING]
> VM only. Do not casually install this experimental driver on your daily-use
> Windows machine.

## Current State

The repo has a WDM driver project, but it does not have a full driver package
yet:

- no `.inf` file
- no install script
- no remove script
- no automated signing flow
- no VM deployment automation

The commands below are beginner-friendly manual notes for future testing. They
assume you already built a `.sys` file and copied it into a disposable Windows
VM.

## Service Name

Use this service name for manual testing unless the project later picks a
different one:

```text
KrnlHardwareInfo
```

Example driver path inside the VM:

```text
C:\KrnlHardwareInfo\KrnlHardwareInfoDriver.sys
```

## Open An Admin Terminal

Open Command Prompt or PowerShell as Administrator inside the VM.

Kernel driver service commands need admin rights. If a command fails with
`Access is denied`, you are probably not elevated.

## Install The Driver Service

```powershell
sc.exe create KrnlHardwareInfo type= kernel start= demand binPath= "C:\KrnlHardwareInfo\KrnlHardwareInfoDriver.sys"
```

Notes:

- The spaces after `type=`, `start=`, and `binPath=` are required by `sc.exe`.
- `start= demand` means Windows will not start it automatically at boot.
- The `binPath` must point to the real `.sys` path inside the VM.

## Start The Driver

```powershell
sc.exe start KrnlHardwareInfo
```

Then check the service state:

```powershell
sc.exe query KrnlHardwareInfo
```

If the driver starts successfully, the app should be able to open:

```text
\\.\KrnlHardwareInfo
```

## Run The App

After the driver starts, run the user-mode app from the VM.

The current app only does a small smoke test:

- open the driver device
- query driver status
- query CPUID leaf `0`

If the app says it cannot open the driver, check that the service started and
that the device name still matches the shared contract.

## Stop The Driver

```powershell
sc.exe stop KrnlHardwareInfo
```

Then check that it stopped:

```powershell
sc.exe query KrnlHardwareInfo
```

## Remove The Driver Service

Stop it first, then delete the service entry:

```powershell
sc.exe delete KrnlHardwareInfo
```

After deleting it, query should fail or report that the service does not exist:

```powershell
sc.exe query KrnlHardwareInfo
```

## Clean Up

After testing:

- stop the driver
- delete the service
- remove copied test files if you do not need them
- restore the VM snapshot/checkpoint if anything looks weird

## Common Beginner Errors

### `The specified service already exists`

You already ran `sc.exe create`. Either use the existing service or delete it
first:

```powershell
sc.exe delete KrnlHardwareInfo
```

### `The system cannot find the file specified`

The `binPath` is wrong, or the `.sys` was not copied into the VM where the
service expects it.

### `The service cannot be started`

Common causes:

- the driver is unsigned or not test-signed
- test-signing mode is off
- the WDK build output is not the file you think it is
- the driver failed during `DriverEntry`

Check `docs/test-signing.md` before trying random fixes.

### The app still cannot open the device

The driver service may exist but not be running, or `DriverEntry` may have
failed before creating the device. Check:

```powershell
sc.exe query KrnlHardwareInfo
```
