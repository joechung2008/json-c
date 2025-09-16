# PowerShell script to run the CLI on Windows
param(
    [string]$Configuration = 'Debug'
)

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'

# Prefer the configuration-specific output directory (e.g. build\Debug\json-cli.exe)
$ConfigOut = Join-Path $BuildDir $Configuration
$Exe = Join-Path $ConfigOut 'json-cli.exe'

if (-not (Test-Path $Exe)) {
    # Fallback to root build directory (older CMake generator layouts)
    $Exe = Join-Path $BuildDir 'json-cli.exe'
}

if (-not (Test-Path $Exe)) {
    Write-Host "Executable not found: $Exe"
    Write-Host "Run .\bin\build.ps1 first."
    exit 1
}

# Ensure the folder containing the built DLL(s) is on PATH so the exe can find json_c.dll
# Common locations: build\<Config> and build\tests\<Config>
if (Test-Path $ConfigOut) {
    $env:PATH = "$ConfigOut;$env:PATH"
}
$TestsOut = Join-Path (Join-Path $BuildDir 'tests') $Configuration
if (Test-Path $TestsOut) {
    $env:PATH = "$TestsOut;$env:PATH"
}

Write-Host "Running: $Exe with args: $args"
& $Exe @args