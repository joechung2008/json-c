# PowerShell build script for Windows
param(
    [string]$Configuration = 'Debug'
)
$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'
if (-not (Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir | Out-Null }
cmake -S $RootDir -B $BuildDir -DCMAKE_BUILD_TYPE=$Configuration
cmake --build $BuildDir --config $Configuration --parallel
Write-Host "Build complete. Artifacts in: $BuildDir"