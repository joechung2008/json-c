# Guidelines for AI Coding Assistants

## General Instructions

- Be concise and direct. Avoid preambles, acknowledgments, or conversational pleasantries.
- Provide only the answer. Do not include explanations unless specifically asked.
- Limit responses to the core information requested.
- Never ask follow-up questions or offer further assistance.

## Testing

- Do not combine multiple cases into one. Each test should focus on a single aspect or behavior.
- Ensure tests cover edge cases and potential failure points.
- JSON tests should be grouped by JSON type: Arrays, Numbers, Objects, Pairs, Strings, Values.
- Run all tests before committing code to ensure nothing is broken.
- Use cmocka for unit testing and follow best practices.
- Use descriptive names for test cases.
- Write tests for all new features and bug fixes.

## Building and Running

- Use CMake for building the project.
- See README.md "Build and run" section for details.
- Use bin/format.sh (bash) or bin/format.ps1 (PowerShell) for formatting with clang-format.
- Use bin/lint.sh (bash) or bin/lint.ps1 (PowerShell) for linting with clang-tidy.
- Prefer bin scripts over direct tool invocation for platform-specific setup.
