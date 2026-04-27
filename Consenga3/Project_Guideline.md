# Project Reorganization Guide

## Purpose

This document is a generic guide for reorganizing small and medium C/C++ projects that started as sparse, mixed, or poorly structured code.

The goal is to improve readability and maintainability without introducing unnecessary complexity.

Preferred principles:

- small modules
- clear `.h` and `.cpp` separation
- plain `struct` data where possible
- only the necessary constructors
- simple helper functions instead of heavy abstractions

## Typical Problems in Weakly Structured Projects

Many small projects begin in a single file or in a few loosely organized files. Common problems are:

- one file contains too many responsibilities
- headers contain full implementations instead of only declarations
- data structures and algorithms are mixed together
- input/output code is mixed with business logic
- generated files are stored beside source files
- demo code, test code, and reusable code are not separated
- global variables are used too broadly

These problems do not always make a project incorrect, but they make it harder to understand, test, and extend.

## Main Reorganization Goal

The main objective is to give each file one clear role.

A good reorganization should make the project:

- easy to read
- easy to compile
- easy to test
- easy to extend
- easy to navigate

The result should feel predictable, not advanced.

## Recommended Target Layout

For many small and medium projects, a structure like this is enough:

```text
project/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── data_types.h
├── data_types.cpp
├── algorithms.h
├── algorithms.cpp
├── io.h
├── io.cpp
└── build/
```

This layout is only a model. File names can change depending on the project, but the separation of responsibilities should remain clear. For small projects, keeping source files at the project root is often enough.

## CMake Compilation

Prefer compiling through CMake instead of calling `g++` directly.

CMake keeps compiler options, source files, and build commands in one repeatable place. This is useful when the project grows beyond one file.

Typical workflow:

```sh
cmake -S . -B build
cmake --build build
```

Run the executable from the `build/` directory or define a custom CMake target if the project needs a repeatable run command.

Recommended `CMakeLists.txt` responsibilities:

- define the project name
- set the C or C++ standard
- list all source files used by the executable
- enable useful compiler warnings
- keep build files inside `build/`

## Responsibility of Each Area

### `main.cpp`

The main file should only coordinate program execution.

Good content:

- parse command-line arguments
- initialize the program
- call core functions
- print final results

Avoid placing large algorithms or large data structure logic directly in `main.cpp`.

### `build/`

This directory should contain build artifacts only.

It should stay separate from source files.

## Suggested Module Split

A simple project can usually be divided into modules like these:

### Data Module

Use one module for core data structures.

Good content:

- `struct` definitions
- basic creation/init functions
- small utility functions tightly coupled to the data type

Example role:

- node
- list
- queue
- stack
- record

### Algorithm Module

Use one module for reusable processing logic.

Good content:

- searching
- sorting
- traversal
- validation
- calculations on the core data structures

This module should focus on behavior, not on presentation or file export.

### I/O Module

Use one module for input/output responsibilities.

Good content:

- file reading
- file writing
- text export
- formatted printing

This keeps the core logic independent from console and file details.

### Utility Module

Use this only if there are shared helpers used by more than one part of the project.

Good content:

- small formatting helpers
- shared parsing helpers
- simple conversions

Avoid creating a generic utility module too early. Add it only when duplication is real.

## Practical Reorganization Rules

When cleaning an existing project, a safe order is:

1. Identify the main responsibilities currently mixed together.
2. Separate declarations from implementations.
3. Move reusable logic out of the main file.
4. Isolate input/output and generated-output code.
5. Keep generated artifacts outside source files.
6. Build through CMake using the `build/` directory.

This order reduces confusion and avoids large uncontrolled refactors.

## Simple Style Rules

To keep the result easy to understand, prefer these rules:

- one module = one clear responsibility
- header files declare, source files implement
- avoid large inline implementations in headers
- minimize global variables
- pass context explicitly to functions when practical
- use short, descriptive names
- write comments for intent, not for obvious lines
- prefer simple flow over clever tricks

## About Constructors and Data Design

Do not introduce complicated class hierarchies unless the project truly needs them.

Prefer:

- plain `struct` for simple data containers
- explicit initialization functions when enough
- one basic constructor only if it removes repeated setup clearly

Good direction:

- simple data holders
- explicit setup
- readable ownership

Avoid:

- deep inheritance
- excessive object-oriented design
- multiple constructors for trivial data
- patterns added only for style

The general rule is to use the minimum language features needed to improve clarity.

## Minimal Example of a Clean Split

```cpp
// data_types.h
typedef struct item {
    int value;
    struct item *next;
} item_t;

item_t *item_create(int value);
void item_destroy(item_t *item);
```

```cpp
// algorithms.h
int compute_result(item_t *root);
bool validate_structure(item_t *root);
```

```cpp
// main.cpp
int main(int argc, char **argv) {
    // parse options
    // prepare input
    // call core logic
    // print results
    return 0;
}
```

## File Naming Guidance

Choose names that reflect responsibility.

Good examples:

- `data_types.h`
- `algorithms.cpp`
- `io.cpp`
- `parser.cpp`
- `main.cpp`

Avoid names that are too generic unless the project is extremely small.

## Final Guideline

When reorganizing a project with weak structure, do not try to make it look sophisticated. Make it clear.

A good result is based on:

- small files
- clear responsibilities
- simple data structures
- isolated logic
- isolated input/output
- minimal but useful abstractions

For many projects, simple modular C/C++ is the best solution.
