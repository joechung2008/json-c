# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [Unreleased]

## 2025-10-02

- Add newline to CLI output
- Fix .vscode/tasks.json
- Fix Dockerfile
- Remove JSON_C_BUILD_TESTS flag; build tests always
- Simplify build
- Update .github/copilot-instructions.md
- Update tasks.json

## 2025-10-01

- Dedupe test case declarations
- Disable generating compile-commands.json on Windows due to symlink issues
- Fix includes
- Fix lint configuration
- Fix pretty printing

## 2025-09-30

- Add .dockerignore
- Add `suppress_leading_indent` parameter to `print_token`
- Add prerequisites to README
- Add printer tests
- Address lint errors
- Autoformat recent changes
- Configure Windows builds with -W3 instead of -Wall
- Fix bug in json_value_type
- Fix build errors
- Fix indentation when printing pair value arrays or objects
- Fix test function declarations
- Improve json_text to pretty-print json_parse output
- Remove redundant 'type' output; include 'key' and 'value' output when outputting pairs
- Rename json_compat.c to json_internal.c
- Set -Wall in local build scripts
- Update tests not to use json_text for assertions

## 2025-09-29

- Add Dockerfile

## 2025-09-23

- Add braces around single-line blocks in main.c

## 2025-09-16

- Add code formatting scripts
- Change .clang-tidy configuration
- Disable noisy lint rules
- Disable noisy lint rules (2)
- Enable cert and security lint checks
- Enable cert and security lint checks (2)
- Enable CI
- Enable linting
- Fix bugprone-branch-clone in token_free.c
- Fix CI to run tests on Linux builds
- Fix CMake and PowerShell scripts for Visual Studio 2022
- Fix Linux builds to enable testing
- Fix Linux and Windows tasks in tasks.json
- Fix strnlen warning on Linux (gcc)
- Modernize code to C17 (#1)
- Recommend VS Code extensions for C and CMake
- Simplify bin/lint.ps1 and bin/lint.sh
- TESTS: Add ASan troubleshooting and interpretation guide
- Update lint.ps1 for C17
- Use C17 instead of C11
