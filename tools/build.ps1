param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"

Write-Host "KRNL Hardware Info build helper"
Write-Host "Configuration: $Configuration"
Write-Host "Platform: $Platform"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$solution = Join-Path $repoRoot "KrnlHardwareInfo.sln"

if (-not (Test-Path -LiteralPath $solution)) {
    Write-Warning "No solution file was found at $solution."
    exit 1
}

function Find-MsBuild {
    $fromPath = Get-Command msbuild.exe -ErrorAction SilentlyContinue
    if ($fromPath) {
        return $fromPath.Source
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path -LiteralPath $vswhere) {
        $found = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\Current\Bin\MSBuild.exe"
        if ($LASTEXITCODE -eq 0 -and $found) {
            return $found[0]
        }
    }

    return $null
}

$msbuild = Find-MsBuild
if (-not $msbuild) {
    Write-Warning "MSBuild was not found. Open a Visual Studio Developer Command Prompt or install Visual Studio Build Tools."
    Write-Host "See docs/build.md for the beginner build notes."
    exit 1
}

Write-Host "MSBuild: $msbuild"
& $msbuild $solution /m /p:Configuration=$Configuration /p:Platform=$Platform
