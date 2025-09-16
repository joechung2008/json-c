# Guidelines for AI coding assistants like GitHub Copilot

## Testing

- Do not combine multiple cases into one.
  - Each test should focus on a single aspect or behavior.
- Ensure tests cover edge cases and potential failure points.
- JSON tests should be grouped by JSON type:
  - Arrays
  - Numbers
  - Objects
  - Pairs (key/value pairs within objects)
  - Strings
  - Values (null, false, true)
- Run all tests before committing code to ensure nothing is broken.
- Use a consistent testing framework and follow best practices for writing tests.
- Use descriptive names for test cases to clearly indicate their purpose.
- Write tests for all new features and bug fixes.

## Running and building in this repo

See the project's `README.md` "Build and run" section for the canonical list of `bin/` helper scripts and usage examples (PowerShell and bash). The `bin` scripts handle configuration, platform detection, and DLL path setup on Windows so prefer them over invoking build/test tools directly.
