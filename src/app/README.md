# App

User-mode application code belongs here.

Put the CLI, GUI, background service, driver loading helpers, display formatting, and user-mode driver client code in this folder.

Current foundation files:

- `DriverClient.h` / `DriverClient.cpp`: owns the Win32 device handle and wraps `DeviceIoControl` calls.
- `Logger.h` / `Logger.cpp`: tiny console/debug logging helpers.
- `main.cpp`: minimal console entry point that opens the driver, queries status, and tries a basic CPUID request.

This is not a WinUI 3 project yet. When one is added, keep page/viewmodel code under `src/app` and have it call `DriverClient` instead of calling `DeviceIoControl` directly.

## Future Hardware Monitoring Notes

Future app-side hardware monitoring work belongs here when it is about:

- formatting CPU/sensor/clock values for humans
- CLI output
- GUI pages and view models
- polling and refresh behavior
- beginner-friendly error messages

Do not put kernel probing or raw hardware access in the app. If the app needs
driver data, add a clear method to `DriverClient` after the shared contract and
driver handler are designed.
