Running tests (cross-platform)

This project uses CMake and cmocka for unit tests. The tests are configured to fetch cmocka automatically if it is not present on the system (FetchContent pinned to cmocka v1.1.5), so you do not need to install cmocka manually.

Quick commands (Linux/macOS/Windows with CMake)

Configure and build (out-of-source):

```bash
./bin/build.sh
```

Run tests with CTest (show failures):

```bash
./bin/run-tests.sh
```

Notes and options

- The tests are run as executables built in `build/`. The CMake test setup copies the built `json_c` shared library next to the test executable on Windows and sets RPATH on Unix/macOS so tests can find the library when run from the build tree.

- If you want extra sanitizer checking (AddressSanitizer/UBSan) we can add a CMake option to enable sanitizer compile/link flags for test builds. Note that sanitizer flags are only supported on GCC/Clang (or clang-cl on Windows); MSVC's sanitizer support is limited. Tell me if you'd like that added.

Troubleshooting

- If the FetchContent download fails (e.g., no network), you can install cmocka on the system and rerun CMake; the top-level `tests/CMakeLists.txt` prefers an installed cmocka when available.

- If tests fail with a runtime loader error about missing shared libraries on Windows, ensure the `build/` step finished; CMake is set to copy the `json_c` DLL next to tests, but if you're running tests from a custom location you may need to set `PATH` to include `build/`.

The `bin/` directory contains helper scripts (`bin/build.sh`, `bin/run-tests.sh`, etc.) you can use to build and run the test suite.
