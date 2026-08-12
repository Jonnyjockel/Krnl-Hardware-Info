param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64",
    [string]$Destination = ""
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")

# Default destination if not provided: a folder next to the repo, clearly named.
if (-not $Destination) {
    $Destination = Join-Path $repoRoot "vm-deploy"
}

$appExe = Join-Path $repoRoot "build\KrnlHardwareInfoApp\$Platform\$Configuration\KrnlHardwareInfo.exe"
$driverSys = Join-Path $repoRoot "build\KrnlHardwareInfoDriver\$Platform\$Configuration\KrnlHardwareInfoDriver.sys"

Write-Host "KRNL Hardware Info VM deploy helper"
Write-Host "Configuration : $Configuration"
Write-Host "Platform      : $Platform"
Write-Host "Destination   : $Destination"
Write-Host ""

# --- Validate source files exist (with beginner-friendly messages) ---
if (-not (Test-Path -LiteralPath $appExe)) {
    Write-Warning "App executable not found: $appExe"
    Write-Warning "Build the app first: .\tools\build.ps1 -Configuration $Configuration -Platform $Platform"
    exit 1
}

# --- Create destination and copy ---
New-Item -ItemType Directory -Force -Path $Destination | Out-Null

Write-Host "Copying app executable..."
Copy-Item -LiteralPath $appExe -Destination $Destination -Force
Write-Host "  -> $appExe"

$copiedDriver = $false
if (Test-Path -LiteralPath $driverSys) {
    Write-Host "Copying driver binary..."
    Copy-Item -LiteralPath $driverSys -Destination $Destination -Force
    Write-Host "  -> $driverSys"
    $copiedDriver = $true
} else {
    Write-Warning "Driver binary not found: $driverSys"
    Write-Warning "The driver has not been built yet (needs WDK). Only the app was copied."
}

Write-Host ""
Write-Host "Deploy folder contents:"
Get-ChildItem -LiteralPath $Destination -File | Select-Object Name, Length | Format-Table -AutoSize

Write-Host "Next steps inside the VM (manual, per docs\vm-deployment-notes.md):"
Write-Host "  1. Copy the deploy folder into the VM (e.g. C:\KrnlHardwareInfo)."
Write-Host "  2. Take a VM snapshot before installing the driver."
if ($copiedDriver) {
    Write-Host "  3. Enable test-signing in the VM (docs\test-signing.md), then install/start the driver service."
} else {
    Write-Host "  3. No driver binary present yet - driver install steps come later."
}
