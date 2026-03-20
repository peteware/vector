---
name: run-tests
description: Build and run tests, optionally filtering by test name
argument-hint: "[filter]"
---

Build the project and run tests.

Steps:
1. Build: `cmake --build cmake-build-debug`
2. Run tests with CTest:
   - If filter provided: `cd cmake-build-debug && ctest --extra-verbose -R "$ARGUMENTS"`
   - Otherwise: `cd cmake-build-debug && ctest --extra-verbose`
3. Report pass/fail results