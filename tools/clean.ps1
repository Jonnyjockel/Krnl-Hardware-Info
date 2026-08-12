param(
    [switch]$Execute
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$candidateNames = @("build", "out", "artifacts")

Write-Host "KRNL Hardware Info clean helper"

$found = @()
foreach ($name in $candidateNames) {
    $path = Join-Path $repoRoot $name
    if (Test-Path -LiteralPath $path) {
        $found += $path
        Write-Host "Found: $path"
    }
}

if ($found.Count -eq 0) {
    Write-Host "Nothing to clean."
    exit 0
}

if (-not $Execute) {
    Write-Host ""
    Write-Host "Dry run only. Nothing was deleted."
    Write-Host "To actually delete, run with -Execute. You will be prompted for real confirmation."
    exit 0
}

# --- REAL blocking confirmation guard (Jonny rule) ---
# 1) Refuse if input is not interactive: there is no human present to type y/n.
if ([Console]::IsInputRedirected) {
    Write-Warning "Refusing to delete: input is not interactive (no human present to confirm)."
    Write-Warning "Run this script from a real terminal where a human can type y/n."
    exit 1
}

# 2) Block on real input. No default, no -Force, no self-answer.
Write-Host ""
Write-Host "The following will be PERMANENTLY DELETED:"
$found | ForEach-Object { Write-Host "  - $_" }
Write-Host ""

$answer = Read-Host "Type 'y' to confirm permanent deletion (anything else aborts)"
if ($answer -ne "y" -and $answer -ne "yes") {
    Write-Host "Aborted. Nothing was deleted."
    exit 1
}

# --- confirmed by a human ---
foreach ($path in $found) {
    Write-Host "Deleting: $path"
    Remove-Item -LiteralPath $path -Recurse -Force
}

Write-Host "Clean complete."
