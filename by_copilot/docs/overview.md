# Project Overview

This project source code for building a WebAssembly (WASM) interpreter in C90.

## Components

- **Interpreter**: Executes parsed WASM modules. See `src/wasm_interpreter.c`.
- **Parser**: Reads WASM binary format into internal structures. See `src/wasm_parser.c`.
- **Utilities**: Helper functions for development and debugging. See `src/util.c`.
- **Types**: Data structures for representing WASM modules. See `include/wasm_types.h`.

## Build & Test

Run `make` to build and `make test` to execute basic tests.

## Next steps for the WASM interpreter project in C90:

### 1. Expand Data Structures

    Add detailed fields to the WasmModule struct for sections like types, imports, exports, functions, tables, memories, and globals.
    Define structures for WASM instructions, value types, and sections as per the WebAssembly binary format.

### 2. Implement Section Parsers

    In wasm_parser.c, add functions to parse each WASM section (types, imports, functions, exports, code, memory, globals).
    Write test cases in tests/test_parser.c for each section parser.

### 3. Develop Interpreter Loop

    In wasm_interpreter.c, add an instruction dispatch loop that fetches, decodes, and executes WASM instructions using wasm_instructions.c.
    Support basic WASM opcodes (e.g., NOP, UNREACHABLE, CONST, ADD, SUB).
    Use wasm_memory.c and wasm_globals.c for memory/global instructions.

### 4. Implement Minimal Execution

    Implement a minimal stack-based execution model to run simple WASM functions.
    Handle function calls, returns, and basic arithmetic.

### 5. Add Example WASM Binaries

    Create or download minimal WASM binaries for testing.
    Add example test cases to evaluate parser and interpreter correctness.

### 6. Improve Error Handling & Logging

    Add robust error checking and clear diagnostics/logging throughout the codebase.

### 7. Documentation

    Update README.md and docs/overview.md with usage, design decisions, and roadmap.

### 8. Continuous Testing

    Expand the Makefile to automate building and running tests.
    Consider using CI (GitHub Actions) for automatic test runs on pushes/PRs.


