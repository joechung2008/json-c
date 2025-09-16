# json-c

## License

MIT

## Reference

[json.org](http://json.org)

## Build and run

This project uses CMake. The instructions below use an out-of-source build directory named `build`.

Configure the build and build the code:

```bash
# POSIX / bash
./bin/build.sh
```

```bash
# PowerShell (Windows)
./bin/build.ps1
```

Run the CLI (from the project root):

```bash
# POSIX / bash
./bin/run-cli.sh
```

```powershell
# PowerShell (Windows)
./bin/run-cli.ps1
```

Run unit tests (see `TESTS.md` for more details):

```bash
# POSIX / bash
./bin/run-tests.sh
```

```powershell
# PowerShell (Windows)
./bin/run-tests.ps1
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
# Generate compilation database
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Run clang-tidy across C sources (uses .clang-tidy at repo root if present)
clang-tidy -p build $(find src -name '*.c')

# Or use the bundled helper (parallel when possible)
bin/lint.sh -p build -j 4
```

If you want to analyze headers as well, prefer `run-clang-tidy.py` (bundled with LLVM) or use `bin/lint.sh` which will attempt to map headers to translation units. See `TESTS.md` for more information on interpreting analyzer output.

More information:

- clang-tidy docs: https://clang.llvm.org/extra/clang-tidy/
- A conservative `.clang-tidy` configuration is included in the repository to reduce noise.

## Notes

- The test setup fetches `cmocka` automatically if it's not available on the system (FetchContent pinned to cmocka 1.1.5). If you prefer using a system-installed cmocka, install it and rerun CMake.

- If you want editor tooling (clangd, clang-tidy) to see compile flags, generate `compile_commands.json` by configuring CMake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`. The file will be created in the `build/` directory. See `TESTS.md` for a symlink example.
