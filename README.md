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

### Windows

- [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) (includes MSVC, CMake, Ninja)
- PowerShell (for format/lint scripts)

## Reference

[json.org](http://json.org)

## Build and run

This project uses CMake with fixed output directories (`out/` for binaries, `obj/` for archives).

### Build

```bash
# Configure and build (default/unoptimized)
cmake -S . -B build
cmake --build build

# Release build (optimized)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Debug build (with debug symbols)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Run CLI

```bash
./out/json-cli

# Windows
.\out\json-cli.exe
```

### Run tests

```bash
# From project root
ctest --test-dir build --output-on-failure

# On Windows with Visual Studio generator, specify configuration
ctest --test-dir build -C Debug --output-on-failure

# Or run the test executable directly
./out/json_tests
```

## Formatting with clang-format

Format the code using [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html):

```bash
# POSIX / bash (preferred)
./bin/format.sh
```

```powershell
# PowerShell (Windows)
./bin/format.ps1
```

## Linting with clang-tidy and MSVC static analysis

We recommend using static analysis tools for code quality checks.

```bash
# POSIX / bash (clang-tidy)
./bin/lint.sh
```

```powershell
# PowerShell (Windows, MSVC static analysis)
./bin/lint.ps1
```

For clang-tidy manual usage:

```bash
# Generate compilation database
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Run clang-tidy across C sources
clang-tidy -p build $(find src -name '*.c')
```

A conservative `.clang-tidy` configuration is included in the repository. See [clang-tidy docs](https://clang.llvm.org/extra/clang-tidy/) for more information.

## Notes

- The test setup fetches `cmocka` automatically if not available on the system (FetchContent pinned to cmocka 1.1.5).
- For editor tooling (clangd, clang-tidy), generate `compile_commands.json` with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`.
