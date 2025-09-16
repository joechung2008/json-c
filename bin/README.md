This `bin/` helper directory contains convenience scripts for building, linting, and testing the project.

Notes
-----
- `bin/lint.sh` and `bin/lint.ps1` are intentionally minimal helper scripts and take no arguments.
- `bin/lint.sh` (POSIX) expects a compilation database at `build/compile_commands.json` and runs `clang-tidy -p build` per file.
- `bin/lint.ps1` (PowerShell) runs `clang-tidy` per-file without requiring a compilation database (designed for Visual Studio users); it also filters noisy clang-tidy advice and summary lines.

If you prefer a persistent symlink on Windows (so tools like clangd can find `compile_commands.json` at the repo root), enable Developer Mode in Settings -> Update & Security -> For developers, or run PowerShell as Administrator and create the symlink manually:

Example (Administrator or Developer Mode):
New-Item -ItemType SymbolicLink -Path compile_commands.json -Target build/compile_commands.json
