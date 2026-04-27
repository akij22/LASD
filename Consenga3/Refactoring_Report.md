# Refactoring Report

## Goal

The original program in `consegna3.cpp` was reorganized following `Project_Guideline.md`.

The behavior of the graph construction and DFS cycle detection was preserved:

- the demo graph still has 7 nodes
- node values are still `2 * i`
- each node still has the edge `i -> (i + 1) % n_nodi`
- DFS still starts from node `0`
- the program still reports `ciclo? 1`
- the optional DOT output still writes `graph.dot`

## New Project Structure

```text
.
├── CMakeLists.txt
├── Project_Guideline.md
├── Refactoring_Report.md
├── main.cpp
├── options.h
├── options.cpp
├── linked_list.h
├── linked_list.cpp
├── graph.h
├── graph.cpp
├── dfs.h
├── dfs.cpp
├── dot_output.h
└── dot_output.cpp
```

## Main Changes

### Build System

Added `CMakeLists.txt`.

The project can now be built with:

```sh
cmake -S . -B build
cmake --build build
```

This replaces one-off `g++` compilation with a repeatable build configuration.

### Removed Broad Global State

The original file used global variables for:

- graph nodes
- visit flags
- adjacency lists
- graph output file
- DFS visit counter
- graph-print operation counter
- command-line options

These responsibilities are now stored in explicit structs:

- `Graph`
- `DotContext`
- `ProgramOptions`

Functions receive the data they need through parameters instead of depending on unrelated globals.

### Split Responsibilities

The old single source file mixed data structures, graph logic, DFS, command-line parsing, console output, and DOT output.

It was split into focused modules:

- `main.cpp`: coordinates program execution
- `options.*`: command-line option parsing
- `linked_list.*`: linked-list data structure
- `graph.*`: graph creation, destruction, and adjacency printing
- `dfs.*`: DFS traversal and cycle detection
- `dot_output.*`: DOT file generation

### Cleaned Unused Code

Removed code that was unrelated to this graph/DFS program:

- unused operation counters from other exercises
- unused array-printing helpers
- unused sorting/searching variables
- unused C headers
- empty `list_delete` placeholder

The new `list_delete` actually releases all list nodes and the list object.

### Kept Simple Data Design

The refactor keeps plain structs and simple functions instead of introducing classes or inheritance.

This follows the guideline preference for:

- small modules
- explicit setup
- simple ownership
- minimal abstraction

## Logic Preservation Notes

The graph is still a directed cycle:

```text
0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 0
```

DFS still uses the same visit-state meaning:

- `0`: not visited
- `1`: currently being visited
- `2`: completely processed

Finding a node with state `1` still means a cycle was found.

## Improvements

- The code is easier to navigate because each file has one clear role.
- Build artifacts can stay inside `build/`.
- Memory allocated for lists and graph arrays is now released before exit.
- DOT output state is isolated from the DFS logic.
- The program is easier to extend with different graph creation functions later.
