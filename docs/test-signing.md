# Test-Signing Notes

Windows does not normally load random unsigned kernel drivers. That is a good
thing. Kernel drivers run with very high privileges, so Windows expects them to
be signed.

For development, people usually use test-signed drivers in a disposable VM.
That lets you test your own driver without pretending it is production-ready.

> [!WARNING]
> Do this in a VM. Do not enable test-signing casually on your daily-use PC.

## Why Unsigned Drivers Usually Do Not Load

A `.sys` file is kernel code. If Windows allowed any unsigned driver to load by
default, every mistake and every malicious file would get a very sharp tool.

So a normal Windows install requires driver signing. If the KRNL driver is not
signed in a way Windows accepts, `sc.exe start` can fail even if the service was
created correctly.

## What Test-Signing Mode Is

Test-signing mode is a Windows boot setting for driver development. When it is
enabled, Windows allows test-signed kernel drivers to load.

It is meant for development and testing, not normal daily use.

You can check the current setting with:

```powershell
bcdedit /enum
```

Look for `testsigning`.

## Turn Test-Signing On In A VM

Open an elevated terminal inside the VM:

```powershell
bcdedit /set testsigning on
```

Then reboot the VM.

After rebooting, Windows should show that it is in test mode. That is expected
inside a development VM.

## Turn Test-Signing Off Again

Open an elevated terminal:

```powershell
bcdedit /set testsigning off
```

Then reboot.

Use this when you are done with driver testing or before returning a VM to a
cleaner state.

## Why Test-Sign A Driver For Development

Test-signing gives you a normal development loop:

- build the driver
- sign it with a local test certificate
- copy it to a disposable VM
- load it through normal Windows driver/service tools
- test it
- stop/remove it
- restore the VM snapshot if needed

This repo now has a test-cert helper: `tools\make-test-cert.ps1`. Run it with `-DryRun` first to see exactly what it does; without `-DryRun` it creates a self-signed CodeSigning certificate in the current user's store and exports `krnl-test.cer` / `krnl-test.pfx` to `build\testcert\`. Run it where you actually want the certificate (ideally in the test VM workflow).

After the certificate exists and the VM has test-signing enabled, sign the built driver with `signtool` (from the WDK):

```powershell
signtool sign /f build\testcert\krnl-test.pfx /p KrnlTestCert1! /fd SHA256 /t http://timestamp.digicert.com build\KrnlHardwareInfoDriver\x64\Debug\KrnlHardwareInfoDriver.sys
```

(Adjust the password if you passed a different `-PfxPassword` to `make-test-cert.ps1`.)

Signing and deployment automation beyond that is a future workflow item, not something to fake.

## What Not To Do

Do not:

- look for driver-signing bypasses
- disable security features on a personal machine just to experiment
- load random drivers from the internet
- treat a test-signed driver like a production driver
- leave test-signing enabled on a daily-use PC

Test-signing is for controlled development. Keep it boring, visible, and inside
a VM.

## Common Beginner Errors

### `The driver is blocked from loading`

The driver is probably not signed in a way the VM accepts, or test-signing mode
is off.

### `bcdedit` says access is denied

Run the terminal as Administrator.

### Test mode is still visible after testing

Turn test-signing off:

```powershell
bcdedit /set testsigning off
```

Then reboot.
