param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"

Write-Host "KRNL Hardware Info build helper"
Write-Host "Configuration: $Configuration"
Write-Host "Platform: $Platform"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$solutions = Get-ChildItem -LiteralPath $repoRoot -Recurse -Filter "*.sln" -File
$projects = Get-ChildItem -LiteralPath $repoRoot -Recurse -File |
    Where-Object { $_.Extension -in @(".vcxproj", ".wixproj") }

if ($solutions.Count -eq 0 -and $projects.Count -eq 0) {
    Write-Warning "NOT_IMPLEMENTED: no Visual Studio solution or project files exist yet."
    Write-Host "Add a user-mode app project and a WDK driver project before this script can build anything."
    exit 0
}

Write-Warning "NOT_IMPLEMENTED: wire this script to MSBuild once project files are added."
Write-Host "Found $($solutions.Count) solution file(s) and $($projects.Count) project file(s)."
