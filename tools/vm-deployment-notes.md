# VM Deployment Notes

This project should be tested in a disposable Windows VM.

> [!WARNING]
> Do not use your daily-use PC for live driver testing.

## Safe Manual Workflow

1. Build on your dev machine.

   Build the app and driver using the Visual Studio / MSBuild notes in
   `docs/build.md`.

2. Copy files to the VM.

   Copy the user-mode app and built driver output into a simple folder inside
   the VM, for example:

   ```text
   C:\KrnlHardwareInfo\
   ```

3. Take a VM snapshot/checkpoint.

   Do this before installing or starting the driver. If something goes sideways,
   restore the snapshot instead of trying to manually untangle everything.

4. Enable test-signing only if needed.

   See `docs/test-signing.md`. Test-signing is a VM development setting, not a
   daily-use PC setting.

5. Install and start the driver manually.

   Use the normal `sc.exe` workflow in `docs/driver-service.md`.

6. Run the app and test the current smoke path.

   The current app should try to open the device, query driver status, and query
   CPUID.

7. Stop and remove the driver.

   Stop the service, delete the service entry, and confirm the device no longer
   opens.

8. Restore the VM snapshot if needed.

   If anything feels messy, revert. That is the whole point of doing this in a
   VM.

## Helper Scripts Status

- `tools\deploy-to-vm.ps1` — copies the built app (and driver, when built) into a deploy folder.
- `tools\make-test-cert.ps1` — creates a local self-signed test certificate for test-signing.
- `tools\tools-tests.ps1` — dry-run smoke tests for the tools.

Still future / not started:

- installing and removing the driver service through normal `sc.exe` commands (deliberately not started yet)
- capturing driver logs
- cleaning copied test files

Do not add driver-signing bypasses, stealth loading, or anything that hides what
the driver is doing.
