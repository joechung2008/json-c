# PowerShell build script for Windows
param(
    [string]$Configuration = 'Debug',
    [switch]$BuildTests
)

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'
if (-not (Test-Path $BuildDir)) { New-Item -ItemType Directory -Path $BuildDir | Out-Null }
## Force Visual Studio 2022 (Visual Studio 17) generator
$Generator = 'Visual Studio 17 2022'

## Choose architecture based on the current process architecture
if ($env:PROCESSOR_ARCHITECTURE -eq 'AMD64') {
    $Arch = 'x64'
}
else {
    $Arch = 'Win32'
}

## Optional: use vswhere to check for an installed VS2022 and warn if not found
$Vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path $Vswhere)) {
    $Vswhere = Join-Path ${env:ProgramFiles} 'Microsoft Visual Studio\Installer\vswhere.exe'
}
if (Test-Path $Vswhere) {
    try {
        $installPath = & $Vswhere -products * -requires Microsoft.Component.MSBuild -version "[17.0,18.0)" -property installationPath -nologo 2>$null
        if (-not [string]::IsNullOrEmpty($installPath)) {
            Write-Host "Found Visual Studio 2022 at: $installPath"
        }
        else {
            Write-Host "Warning: Visual Studio 2022 not found via vswhere; the script will still try to use generator $Generator"
        }
    }
    catch {
        Write-Host "vswhere check failed; proceeding with generator $Generator"
    }
}
else {
    Write-Host "vswhere not found; proceeding with generator $Generator"
}

## Configure and build with the forced generator
# Note: Visual Studio generators are multi-config. Do not pass
# -DCMAKE_BUILD_TYPE at configure time because it is ignored and
# causes warnings like "Ignoring extra path from command line: 'Debug'".
# The configuration (Debug/Release) is selected at build time with --config.
$cmakeArgs = @('-S', $RootDir, '-B', $BuildDir, '-G', $Generator, '-A', $Arch, '-DCMAKE_C_FLAGS="-W3"', '-Wno-dev')
if ($BuildTests) { $cmakeArgs += '-DJSON_C_BUILD_TESTS=ON' }
cmake @cmakeArgs
cmake --build $BuildDir --config $Configuration --parallel
Write-Host "Build complete. Artifacts in: $BuildDir"