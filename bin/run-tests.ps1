# PowerShell script to run tests via CTest
$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'
if (-not (Test-Path $BuildDir)) {
    Write-Host "Build directory not found; running build first..."
    & $PSScriptRoot\build.ps1
}
ctest --test-dir $BuildDir --output-on-failure