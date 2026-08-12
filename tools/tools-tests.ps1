# tools-tests.ps1 — Safe dry-run smoke tests for the tools scripts.
#
# IMPORTANT: this test harness ONLY runs the tools in their non-destructive
# modes. It never runs clean.ps1 -Execute, never creates certificates, and only
# deploys to a throwaway temp directory which it removes afterwards.
# It is safe to run on a normal dev machine.

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$tools = Join-Path $PSScriptRoot "."
$passCount = 0
$failCount = 0

function Invoke-ToolTest {
    param(
        [string]$Name,
        [scriptblock]$Body
    )
    Write-Host "=== $Name ==="
    try {
        & $Body
        Write-Host "PASS: $Name"
        $script:passCount++
    } catch {
        Write-Host "FAIL: $Name - $($_.Exception.Message)"
        $script:failCount++
    }
    Write-Host ""
}

# --- Test 1: clean.ps1 dry run (no -Execute) must exit 0 and delete nothing ---
Invoke-ToolTest -Name "clean.ps1 dry-run" -Body {
    $buildDir = Join-Path $repoRoot "build"
    $buildExisted = Test-Path -LiteralPath $buildDir

    & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $tools "clean.ps1")
    if ($LASTEXITCODE -ne 0) { throw "clean.ps1 dry-run exited $LASTEXITCODE (expected 0)" }

    if ($buildExisted -and -not (Test-Path -LiteralPath $buildDir)) {
        throw "clean.ps1 dry-run deleted build/ - that must never happen without -Execute"
    }
}

# --- Test 2: clean.ps1 -Execute with redirected input must REFUSE (exit 1) ---
Invoke-ToolTest -Name "clean.ps1 -Execute refusal (no human)" -Body {
    $buildDir = Join-Path $repoRoot "build"
    $buildExisted = Test-Path -LiteralPath $buildDir

    # Redirect stdin so [Console]::IsInputRedirected is true -> must refuse.
    $null = "" | & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $tools "clean.ps1") -Execute
    if ($LASTEXITCODE -ne 1) { throw "clean.ps1 -Execute with redirected input exited $LASTEXITCODE (expected 1 = refused)" }

    if ($buildExisted -and -not (Test-Path -LiteralPath $buildDir)) {
        throw "clean.ps1 -Execute deleted build/ even though input was redirected!"
    }
}

# --- Test 3: deploy-to-vm.ps1 copies app to a temp folder ---
Invoke-ToolTest -Name "deploy-to-vm.ps1 temp copy" -Body {
    $tempDeploy = Join-Path $repoRoot "build\tools-test-deploy"
    if (Test-Path -LiteralPath $tempDeploy) {
        Remove-Item -LiteralPath $tempDeploy -Recurse -Force
    }

    & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $tools "deploy-to-vm.ps1") -Destination $tempDeploy
    if ($LASTEXITCODE -ne 0) { throw "deploy-to-vm.ps1 exited $LASTEXITCODE (expected 0)" }

    $exe = Join-Path $tempDeploy "KrnlHardwareInfo.exe"
    if (-not (Test-Path -LiteralPath $exe)) { throw "deploy-to-vm.ps1 did not copy the app exe" }

    Remove-Item -LiteralPath $tempDeploy -Recurse -Force
}

# --- Test 4: make-test-cert.ps1 dry run must exit 0 and create nothing ---
Invoke-ToolTest -Name "make-test-cert.ps1 dry-run" -Body {
    & powershell -NoProfile -ExecutionPolicy Bypass -File (Join-Path $tools "make-test-cert.ps1") -DryRun
    if ($LASTEXITCODE -ne 0) { throw "make-test-cert.ps1 -DryRun exited $LASTEXITCODE (expected 0)" }

    $certDir = Join-Path $repoRoot "build\testcert"
    if (Test-Path -LiteralPath $certDir) {
        throw "make-test-cert.ps1 -DryRun created build\testcert - dry run must not create files"
    }
}

# --- Summary ---
Write-Host "=============================="
Write-Host "Tools tests: $passCount passed, $failCount failed"
if ($failCount -gt 0) {
    Write-Host "RESULT: FAILED"
    exit 1
}
Write-Host "RESULT: PASSED"
exit 0
