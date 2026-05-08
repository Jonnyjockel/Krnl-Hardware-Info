# Build Notes

This repo now has a first Visual Studio / MSBuild setup, but it is still a
beginner-friendly foundation project. The app side is much closer to normal
"press build" C++. The driver side needs the WDK and still does not include
install, signing, or VM deployment automation.

## Required Tools

- Visual Studio 2022 or Visual Studio Build Tools 2022.
- MSVC v143 C++ build tools.
- Windows 10 or Windows 11 SDK.
- Windows Driver Kit (WDK) that integrates with Visual Studio/MSBuild.

The project files assume:

- `PlatformToolset` `v143` for the user-mode app.
- `PlatformToolset` `WindowsKernelModeDriver10.0` for the driver.
- `WindowsTargetPlatformVersion` `10.0`.
- `x64` Debug and Release builds.

If your installed SDK or WDK uses a more specific version, Visual Studio may ask
to retarget the projects. That is normal. Retargeting is not a new KRNL feature;
it just points MSBuild at the SDK/WDK you actually installed.

## What Builds Now

- `src/app/KrnlHardwareInfoApp.vcxproj` builds the user-mode console app.
- `src/driver/KrnlHardwareInfoDriver.vcxproj` is a WDM kernel driver project
  scaffold for `KrnlDriver.cpp`.
- `KrnlHardwareInfo.sln` includes both projects.

## What Does Not Build / Run Magically Yet

- There is no driver install script.
- There is no driver removal script.
- There is no automated test-signing workflow.
- There is no `.inf` package yet.
- There is no VM deployment automation.
- Building the driver still requires a real WDK install.
- Running the app successfully still requires the driver device to exist.

So yes, the app can compile without the driver being installed, but it will still
print a driver-open failure if the driver is not loaded. That is expected.

## Build the User-Mode App

Open a Developer Command Prompt or x64 Native Tools Command Prompt for Visual
Studio, then run:

```powershell
msbuild src\app\KrnlHardwareInfoApp.vcxproj /p:Configuration=Debug /p:Platform=x64
```

Or build it through the solution:

```powershell
msbuild KrnlHardwareInfo.sln /p:Configuration=Debug /p:Platform=x64
```

There is also a small helper script:

```powershell
.\tools\build.ps1 -Configuration Debug -Platform x64
```

The output goes under:

```text
build\KrnlHardwareInfoApp\x64\Debug\
```

## Build the Driver

Install the WDK first. Then use a Developer Command Prompt that can see the WDK
MSBuild targets and run:

```powershell
msbuild src\driver\KrnlHardwareInfoDriver.vcxproj /p:Configuration=Debug /p:Platform=x64
```

The expected driver output goes under:

```text
build\KrnlHardwareInfoDriver\x64\Debug\
```

> [!WARNING]
> Treat the driver as VM-only. Building a `.sys` is not the same thing as safely
> installing or running it on your personal machine.

## Common Beginner Build Errors

### `msbuild` is not recognized

You are probably in a normal PowerShell window. Open the Visual Studio Developer
Command Prompt or x64 Native Tools Command Prompt instead.

### `The build tools for v143 cannot be found`

Install the MSVC v143 C++ build tools through Visual Studio Installer.

### `Windows SDK version 10.0 was not found`

Install the Windows 10/11 SDK through Visual Studio Installer, then retarget the
solution if Visual Studio asks.

### `WindowsKernelModeDriver10.0` cannot be found

The WDK is missing or not integrated with Visual Studio. Install the WDK that
matches your Windows SDK/Visual Studio setup.

### Driver signing or catalog errors

That part is not fully wired up yet. Driver signing, test certificates, `.cat`
generation, and VM deployment still need their own careful setup. Start with
`test-signing.md` before trying to load the driver in a VM.

### The app builds but says it cannot open the driver

That is normal if the driver is not installed and loaded. The app talks to:

```text
\\.\KrnlHardwareInfo
```

No loaded driver means no device path, so the open call fails.
