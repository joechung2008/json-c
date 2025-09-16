<#
bin/lint.ps1 — run clang-tidy over C sources and headers
Usage: bin\lint.ps1
#>

function Abort($msg) {
    Write-Error $msg;
    exit 2
}

if (-not (Get-Command clang-tidy -ErrorAction SilentlyContinue)) {
    Abort "clang-tidy not found in PATH"
}

$compilerArgsBase = @('--', '-I' + (Join-Path (Get-Location) 'include'), '-std=c11')

$files = Get-ChildItem -Path src, include -Recurse -File -Include *.c, *.h -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }
if ($files.Count -eq 0) {
    Write-Output "No source files found under src/ or include/";
    exit 0
}

foreach ($f in $files) {
    # Use a minimal, hard-coded clang-tidy argument set
    $clangArgs = @('-header-filter=.*')

    # Build argument array and capture output so we can filter noisy advice lines
    $args = @($f) + $clangArgs + $compilerArgsBase
    $output = & clang-tidy @args 2>&1
    $rc = $LASTEXITCODE

    # Filter out clang-tidy advice about header/system-headers and the final "N warnings generated." lines
    $filtered = $output | Where-Object {
        ($_ -notmatch "Use -header-filter") -and
        ($_ -notmatch "Use -system-headers") -and
        ($_ -notmatch "^\s*\d+ warnings generated\.") -and
        ($_ -notmatch "^\s*Suppressed\s+\d+\s+warnings")
    }

    $filtered | ForEach-Object { Write-Output $_ }

    if ($rc -ne 0) { exit $rc }
}
