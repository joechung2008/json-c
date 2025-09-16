# PowerShell script to format .c and .h files with clang-format
# Prefer git-tracked files; fallback to a recursive search.
param(
    [string]$ClangFormat = 'clang-format'
)

function Format-Files($files) {
    if ($files.Count -eq 0) { return }
    # clang-format accepts multiple file arguments; call directly
    & $ClangFormat -i @($files)
}

try {
    if (Get-Command git -ErrorAction SilentlyContinue) {
        $files = git ls-files '*.c' '*.h' | Where-Object { $_ -ne '' }
        if ($files) { Format-Files $files; Write-Host 'Formatting complete.'; exit 0 }
    }
} catch {
    # ignore and fallback
}

# Fallback: find files recursively from current folder excluding build
$files = Get-ChildItem -Recurse -File -Include *.c,*.h -ErrorAction SilentlyContinue | Where-Object { $_.FullName -notmatch '\\build\\' } | ForEach-Object { $_.FullName }
if ($files) { Format-Files $files; Write-Host 'Formatting complete (fallback).' }
else { Write-Host 'No files to format.' }