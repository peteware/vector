# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an educational implementation of `std::vector` targeting C++23. The project reimplements STL containers from scratch to understand their internal workings. It uses the `pw` namespace and follows STL design patterns closely.

## Build System

Uses CMake with C++23 standard. The project is structured as:
- `pw/` - Main library containing STL implementations
- `tests/` - Comprehensive test suite using Catch2 framework

### Common Commands

**Build the project:**
```bash
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug
```

**Run all tests:**
```bash
cd cmake-build-debug && ctest --extra-verbose
```

**Run specific test executable:**
```bash
./cmake-build-debug/tests/unittest
```

**Run individual test components:** Use CLion run configurations or execute specific test binaries from cmake-build-debug/tests/

## Architecture

### Core Structure
- **`pw/vector`** - Main vector header that includes declaration and definition files
- **`pw/impl/`** - Implementation details for STL algorithms and utilities
- **`pw/internal/`** - Private implementation details (Storage class manages memory)
- **Header files without extensions** - Public API mimicking standard library structure

### Key Components
- **Storage**: Internal memory management class in `pw/internal/storage.h` handling allocation, deallocation, and exception safety
- **Vector implementation**: Split between `pw/impl/vector_decl.h` (declarations) and `pw/impl/vector_defn.h` (definitions)
- **STL algorithms**: Custom implementations in `pw/impl/` (copy, move, uninitialized_*, etc.)

### Memory Management
The implementation focuses heavily on exception safety and proper RAII. Storage provides strong exception guarantees during reallocation operations.

## Testing Strategy

Tests are organized into:
- **Unit tests** (`tests/unit/`) - Individual component testing with extensive vector functionality coverage
- **Test utilities** (`tests/test/`) - Custom allocators, type tracking, and test infrastructure
- **Catch2 framework** for test execution

Tests include specialized allocator testing for proper STL allocator model compliance and exception safety verification.

## Development Notes

- No `std` namespace usage except for `<limits>`, `<stdexcept>`, and compiler builtins
- Uses compiler intrinsics for some type traits (`__is_class`, `__is_union`, etc.)
- Extensive template metaprogramming following STL patterns
- Focus on constexpr compatibility throughout