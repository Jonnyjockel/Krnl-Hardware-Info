# App

User-mode application code belongs here.

Put the CLI, GUI, background service, driver loading helpers, display formatting, and user-mode driver client code in this folder.

Current foundation files:

- `DriverClient.h` / `DriverClient.cpp`: owns the Win32 device handle and wraps `DeviceIoControl` calls.
- `Logger.h` / `Logger.cpp`: tiny console/debug logging helpers.
- `main.cpp`: minimal console entry point that opens the driver, queries status, and tries a basic CPUID request.

This is not a WinUI 3 project yet. When one is added, keep page/viewmodel code under `src/app` and have it call `DriverClient` instead of calling `DeviceIoControl` directly.
