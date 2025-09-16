# PowerShell script to run the CLI on Windows
$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'
$Exe = Join-Path $BuildDir 'json-cli.exe'
if (-not (Test-Path $Exe)) {
    Write-Host "Executable not found: $Exe"
    Write-Host "Run .\bin\build.ps1 first."
    exit 1
}
# On Windows just run the exe. If using MSVC, ensure the DLL is next to the exe or in PATH.
& $Exe