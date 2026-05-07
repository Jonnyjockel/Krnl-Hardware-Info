param(
    [switch]$Execute
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$candidateNames = @("build", "out", "artifacts")

Write-Host "KRNL Hardware Info clean helper"

foreach ($name in $candidateNames) {
    $path = Join-Path $repoRoot $name
    if (Test-Path -LiteralPath $path) {
        if ($Execute) {
            Write-Warning "NOT_IMPLEMENTED: deletion is intentionally disabled until build output paths are finalized: $path"
        } else {
            Write-Host "Would clean: $path"
        }
    }
}

if (-not $Execute) {
    Write-Host "Dry run only. Pass -Execute after build output paths are finalized."
}
