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

Format the code:

```bash
# POSIX / bash (preferred)
./bin/format.sh
```

```powershell
# PowerShell (Windows)
./bin/format.ps1
```

Notes

- The test setup fetches `cmocka` automatically if it's not available on the system (FetchContent pinned to cmocka 1.1.5). If you prefer using a system-installed cmocka, install it and rerun CMake.
- If you want editor tooling (clangd, clang-tidy) to see compile flags, generate `compile_commands.json` by configuring CMake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`. The file will be created in the `build/` directory. See `TESTS.md` for a symlink example.
