# json-c

## License

MIT

## Prerequisites

### glibc (Linux, WSL, macOS)

- CMake (version 3.15 or newer recommended)
- GCC (or Clang) toolchain
- GNU Make or Ninja
- glibc development headers (usually included with build-essential)

On Ubuntu/WSL:

```bash
sudo apt update
sudo apt install build-essential cmake
```

### Win32 (Windows)

- [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) (includes MSVC, CMake, Ninja)
- PowerShell

The provided PowerShell scripts will attempt to detect and use Visual Studio 2022 automatically.

## Reference

[json.org](http://json.org)

## Build and run

This project uses CMake. The instructions below use an out-of-source build directory named `build`.

Configure the build and build the code:

```bash
# POSIX / bash
./bin/build.sh
```

```powershell
# PowerShell (Windows)
./bin/build.ps1
./bin/build.ps1 -Configuration Release
```

Note: On Windows `bin/build.ps1` prefers the [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) (CMake generator "Visual Studio 17 2022"). The script will try to detect VS2022 via `vswhere` and will warn if it's not found.

Run the CLI (from the project root):

```bash
# POSIX / bash
./bin/run-cli.sh
```

```powershell
# PowerShell (Windows)
./bin/run-cli.ps1
./bin/run-cli.ps1 -Configuration Release
```

Run unit tests using Criterion (see `TESTS.md` for more details):

```bash
# POSIX / bash
./bin/run-tests.sh
```

```powershell
# PowerShell (Windows)
./bin/run-tests.ps1
./bin/run-tests.ps1 -Configuration Release
```

## Format

Format the code using [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html):

```bash
# POSIX / bash (preferred)
./bin/format.sh
```

```powershell
# PowerShell (Windows)
./bin/format.ps1
```

## Linting with clang-tidy

We recommend using `clang-tidy` for static analysis and style checks. `clang-tidy` uses the C/C++ compile flags from a compilation database (`compile_commands.json`) to perform accurate checks.

Quick start:

```bash
# Generate compilation database (required for bin/lint.sh)
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

```bash
# Run clang-tidy across C sources (uses .clang-tidy at repo root if present)
clang-tidy -p build $(find src -name '*.c')
```

```bash
# POSIX / bash (requires build/compile_commands.json)
./bin/lint.sh
```

```powershell
# PowerShell (Windows)
./bin/lint.ps1
```

See `TESTS.md` for more information on interpreting analyzer output.

More information:

- clang-tidy docs: https://clang.llvm.org/extra/clang-tidy/
- A conservative `.clang-tidy` configuration is included in the repository to reduce noise.

## Notes

- The test setup fetches `Criterion` automatically if it's not available on the system (FetchContent pinned to Criterion v2.4.2). If you prefer using a system-installed Criterion, install it and rerun CMake.

- If you want editor tooling (clangd, clang-tidy) to see compile flags, generate `compile_commands.json` by configuring CMake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`. The file will be created in the `build/` directory. See `TESTS.md` for a symlink example.
