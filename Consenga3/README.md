# Consegna 3

Small C++ graph/DFS example reorganized into focused modules.

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/consegna3
```

Available options:

- `-verbose`: prints detailed list allocation and DFS trace information.
- `-graph`: writes `graph.dot`.

To create a PDF from the DOT file:

```sh
dot graph.dot -Tpdf -o graph.pdf
```
