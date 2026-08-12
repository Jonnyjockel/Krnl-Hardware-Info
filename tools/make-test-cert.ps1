param(
    [string]$Subject = "CN=KRNL Test Signing",
    [string]$OutDir = "",
    [string]$PfxPassword = "KrnlTestCert1!",
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")

if (-not $OutDir) {
    $OutDir = Join-Path $repoRoot "build\testcert"
}

Write-Host "KRNL Hardware Info local test-cert helper"
Write-Host "Subject   : $Subject"
Write-Host "OutDir    : $OutDir"
Write-Host "Dry run   : $($DryRun.IsPresent)"
Write-Host ""

if (-not $DryRun) {
    # Sanity check: creating a cert adds an entry to the CURRENT USER cert store.
    Write-Warning "This creates a self-signed CodeSigning certificate in your CURRENT USER cert store."
    Write-Warning "It is intended for VM-only driver test-signing (see docs\test-signing.md)."
    Write-Warning "Do not use this certificate to sign anything for production."
    Write-Host ""
}

# Commands that will run (shown in dry-run, executed otherwise)
$cert = $null
if ($DryRun) {
    Write-Host "[dry-run] New-SelfSignedCertificate -Type CodeSigningCert -Subject '$Subject' -CertStoreLocation Cert:\CurrentUser\My -KeyExportPolicy Exportable -KeySpec Signature -NotAfter (Get-Date).AddYears(2)"
} else {
    New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

    $cert = New-SelfSignedCertificate `
        -Type CodeSigningCert `
        -Subject $Subject `
        -CertStoreLocation Cert:\CurrentUser\My `
        -KeyExportPolicy Exportable `
        -KeySpec Signature `
        -NotAfter (Get-Date).AddYears(2)

    if (-not $cert) {
        Write-Error "Certificate creation failed."
        exit 1
    }

    $cerPath = Join-Path $OutDir "krnl-test.cer"
    $pfxPath = Join-Path $OutDir "krnl-test.pfx"

    Export-Certificate -Cert $cert -FilePath $cerPath | Out-Null
    $securePass = ConvertTo-SecureString -String $PfxPassword -AsPlainText -Force
    Export-PfxCertificate -Cert $cert -FilePath $pfxPath -Password $securePass | Out-Null

    Write-Host "Certificate thumbprint: $($cert.Thumbprint)"
    Write-Host "Public cert : $cerPath"
    Write-Host "Private key : $pfxPath"
    Write-Host "PFX password: $PfxPassword"
}

Write-Host ""
Write-Host "Next steps (VM only, per docs\test-signing.md):"
Write-Host "  1. Copy krnl-test.cer and krnl-test.pfx into the test VM."
Write-Host "  2. In the VM, import the .cer into Trusted Root + Trusted Publishers."
Write-Host "  3. Enable test-signing in the VM: bcdedit /set testsigning on  (then reboot)."
Write-Host "  4. Sign the driver with signtool (see docs\test-signing.md)."
if ($DryRun) {
    Write-Host ""
    Write-Host "Dry run complete - no certificate was created."
}
