<#
bin/lint.ps1 — run clang-tidy over C sources and headers using compile_commands.json
Usage: bin\lint.ps1 [-BuildDir build] [-Jobs 4] [-- <clang-tidy args>]
#>

param(
    [string]$BuildDir = "build",
    [int]$Jobs = 0,
    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$ExtraArgs
)

function Abort($msg) { Write-Error $msg; exit 2 }

if (-not (Get-Command clang-tidy -ErrorAction SilentlyContinue)) {
    Abort "clang-tidy not found in PATH"
}

if (-not (Test-Path (Join-Path $BuildDir 'compile_commands.json'))) {
    Abort "compile_commands.json not found in $BuildDir. Run: cmake -S . -B $BuildDir -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
}

$files = Get-ChildItem -Path src, include -Recurse -File -Include *.c,*.h -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }
if ($files.Count -eq 0) { Write-Output "No source files found under src/ or include/"; exit 0 }

if ($Jobs -gt 0 -and (Get-Command run-clang-tidy.py -ErrorAction SilentlyContinue)) {
    # run-clang-tidy.py supports -j on Windows under Python.
    & run-clang-tidy.py -p $BuildDir -j $Jobs $files @ExtraArgs
    exit $LASTEXITCODE
}

# Fallback: run clang-tidy sequentially
foreach ($f in $files) {
    & clang-tidy -p $BuildDir $f @ExtraArgs
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
