<#
bin/lint.ps1 — run MSVC static analysis over C sources
Usage: bin\lint.ps1 [-Build <dir>]
#>

param(
    [string]$Build = "build"
)

$BUILD_DIR = $Build

if ($args.Count -ne 0) {
    Write-Error "lint.ps1 takes at most one argument: -Build <dir>. If you want to run MSVC static analysis manually, call MSBuild directly."
    exit 2
}

# Find MSBuild using vswhere
$vswherePath = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswherePath)) {
    Write-Error "vswhere not found. Ensure Visual Studio 2022 is installed."
    exit 2
}

$msbuildPath = & $vswherePath -latest -products * -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuildPath -or -not (Test-Path $msbuildPath)) {
    Write-Error "MSBuild executable not found via vswhere. Ensure Visual Studio 2022 or Build Tools are installed."
    exit 2
}

# Find the main code project files to analyze
$vcxprojFiles = Get-ChildItem -Path $BUILD_DIR -Recurse -Filter *.vcxproj | Where-Object {
    $_.FullName -notlike "*_deps*" -and
    $_.Name -in @("json_c.vcxproj", "json-cli.vcxproj")
}

if ($vcxprojFiles.Count -eq 0) {
    Write-Error "No project files found in $BUILD_DIR. Run: cmake -S . -B $BUILD_DIR"
    exit 2
}

# Run MSBuild with static analysis on each project
foreach ($vcxproj in $vcxprojFiles) {
    Write-Host "Analyzing $($vcxproj.Name)..."

    $msbuildArgs = @(
        $vcxproj.FullName,
        "/t:Build",
        "/p:Configuration=Debug",
        "/p:EnableStaticAnalysis=true",
        "/verbosity:minimal"
    )

    & $msbuildPath $msbuildArgs

    if ($LASTEXITCODE -ne 0) {
        Write-Error "MSVC static analysis failed for $($vcxproj.Name)."
        exit $LASTEXITCODE
    }
}

Write-Host "MSVC static analysis completed successfully."
