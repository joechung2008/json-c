# PowerShell script to run tests via CTest
param(
    [string]$Configuration = 'Debug'
)

$PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Definition
$RootDir = Resolve-Path (Join-Path $PSScriptRoot '..')
$BuildDir = Join-Path $RootDir 'build'

if (-not (Test-Path $BuildDir)) {
    Write-Host "Build directory not found; running build first..."
    & $PSScriptRoot\build.ps1 -Configuration $Configuration -BuildTests
}
else {
    # Ensure tests were configured; run configure with tests enabled to fetch test deps
    & $PSScriptRoot\build.ps1 -Configuration $Configuration -BuildTests | Out-Null
}

# Add config-specific output dir(s) to PATH so tests can find json_c.dll
$ConfigOut = Join-Path $BuildDir $Configuration
if (Test-Path $ConfigOut) {
    $env:PATH = "$ConfigOut;$env:PATH"
}
$TestsOut = Join-Path (Join-Path $BuildDir 'tests') $Configuration
if (Test-Path $TestsOut) {
    $env:PATH = "$TestsOut;$env:PATH"
}

# Also add build/$Configuration to PATH for test executables
if (Test-Path $ConfigOut) {
    $env:PATH = "$ConfigOut;$env:PATH"
}

# Add cmocka dep output to PATH (FetchContent default location)
$CmockaOut = Join-Path $BuildDir "_deps\cmocka-build\src\$Configuration"
if (Test-Path $CmockaOut) {
    $env:PATH = "$CmockaOut;$env:PATH"
} else {
    # also try the src root (some generator layouts put the DLLs here)
    $CmockaRoot = Join-Path $BuildDir '_deps\cmocka-build\src'
    if (Test-Path $CmockaRoot) {
        $env:PATH = "$CmockaRoot;$env:PATH"
    }
}

# Prefer ctest when available
if (Get-Command ctest -ErrorAction SilentlyContinue) {
    # For multi-config generators (Visual Studio) ctest needs the configuration (-C)
    ctest --test-dir $BuildDir -C $Configuration --output-on-failure
} else {
    # Try both build/tests/$Configuration and build/$Configuration for test executables
    $TestDirs = @($TestsOut, $ConfigOut)
    $FoundTests = $false
    foreach ($dir in $TestDirs) {
        if (Test-Path $dir) {
            $TestExes = Get-ChildItem -Path $dir -Filter *.exe -File
            if ($TestExes.Count -gt 0) {
                $FoundTests = $true
                foreach ($test in $TestExes) {
                    Write-Host "Running test: $($test.FullName)"
                    & $test.FullName
                }
            }
        }
    }
    if (-not $FoundTests) {
        Write-Host "No test executables found in $TestsOut or $ConfigOut"
        exit 1
    }
}

# compile_commands.json handling intentionally omitted on Windows