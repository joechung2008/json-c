# Running tests (cross-platform)

This project uses CMake and cmocka for unit tests. The tests are configured to fetch cmocka automatically if it is not present on the system (FetchContent pinned to cmocka v1.1.5), so you do not need to install cmocka manually.

## Quick commands (Linux/macOS/Windows with CMake)

Configure and build (out-of-source):

```bash
./bin/build.sh
```

Run tests with CTest (show failures):

```bash
./bin/run-tests.sh
```

## Notes and options

- The tests are run as executables built in `build/`. The CMake test setup copies the built `json_c` shared library next to the test executable on Windows and sets RPATH on Unix/macOS so tests can find the library when run from the build tree.

- If you want extra sanitizer checking (AddressSanitizer/UBSan) we can add a CMake option to enable sanitizer compile/link flags for test builds. Note that sanitizer flags are only supported on GCC/Clang (or clang-cl on Windows); MSVC's sanitizer support is limited. Tell me if you'd like that added.

## Troubleshooting

- If the FetchContent download fails (e.g., no network), you can install cmocka on the system and rerun CMake; the top-level `tests/CMakeLists.txt` prefers an installed cmocka when available.

- If tests fail with a runtime loader error about missing shared libraries on Windows, ensure the `build/` step finished; CMake is set to copy the `json_c` DLL next to tests, but if you're running tests from a custom location you may need to set `PATH` to include `build/`.

The `bin/` directory contains helper scripts (`bin/build.sh`, `bin/run-tests.sh`, etc.) you can use to build and run the test suite.

## Interpreting AddressSanitizer (ASan) failures

If you enable AddressSanitizer (ASan) and tests fail with sanitizer errors, the output can look noisy at first. The steps below help you triage, reproduce, and fix issues.

1. Reproduce locally with ASan enabled

   - Configure a Debug/ASan build (example using CMake):

```bash
mkdir -p build && cd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" \
    -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" \
    -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address"
cmake --build . -j$(nproc)
```

   - Run the failing test(s) directly to get full sanitizer output (the test runner may swallow some output):

```bash
./tests/json_tests --run test_name
```

2. Read the ASan report top-to-bottom

   - ASan prints a short summary first (error type, address, and thread). Below that is a stack trace for the faulting thread and additional notes (e.g. "heap-use-after-free").
   - Look for the first stack frame in your code (not in libc/stdlib). That frame points to where an invalid access occurred.
   - Example common error types you may see:
     - "heap-use-after-free": Use of memory after it was freed.
     - "heap-buffer-overflow": Indexing past an allocated buffer.
     - "stack-use-after-scope": Returning pointer into a stack buffer that goes out of scope.
     - "double-free or corruption": Memory freed twice or heap metadata corrupted.

3. Use ASAN_OPTIONS to get more detail and fail fast

   - Helpful environment variables:

```bash
export ASAN_OPTIONS=detect_leaks=1:abort_on_error=1:log_path=asan-log
./tests/json_tests --run test_name
```

   - `abort_on_error=1` makes the process exit immediately on error which can make debugging under gdb easier.
   - `log_path` writes the report to a file (useful when CI truncates logs).

4. Reproduce in a debugger (gdb)

   - Run the ASan-instrumented binary under gdb with `catch throw`/`catch` as necessary, or simply run until it aborts:

```bash
gdb --args ./tests/json_tests --run test_name
run
backtrace
```

   - Because ASan aborts on error (with `abort_on_error=1`) you'll usually get a core-like state to inspect with `bt`.

5. Minimize to a small reproducer

   - If the failing test is complex, add a trimmed test or a small program that reproduces the failure; this helps pinpoint the faulty module (parser, token allocator, etc.).

6. Common parser pitfalls we encountered and how they manifest in ASan

   - Premature free / double-free: use-after-free traces where the allocation site and the free site are separate. Fix by consolidating cleanup and avoiding freeing memory twice.
   - Returning pointers into freed or stack-only storage: ASan reports stack-use-after-scope or invalid pointer accesses. Change APIs to copy into heap-owned strings or ensure lifetime is extended.
   - Missing frees (memory leaks): ASan's leak check shows unreachable allocations. Clean up on all error paths.

7. False positives and system libraries

   - Sanitizers sometimes flag issues in system libraries or third-party code. Verify the stack frames: if the first frame inside the process is in your code, it's likely your bug; if it is deep in libc or the test harness, it may be a false positive or a missing suppression.
   - For CI, consider uploading ASan logs for triage instead of showing the raw sanitizer output inline.

8. Add a regression test

   - Once fixed, add a unit test (cmocka) that reproduces the failure and add it to `tests/` so CI prevents regressions.
