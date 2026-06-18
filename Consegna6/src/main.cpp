#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "alignment.h"
#include "file_io.h"
#include "state.h"

// compilazione: g++ consegna6.cpp
//
// Obiettivo:
// 1) osservazione del numero di chiamate ricorsive per fibobacci
// 2) algoritmo di longest common substring
// 3) algoritmo di longest common subsequence

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));

    if (argc != 3) {
        printf("Usage: %s file_versione_1.cpp file_versione_2.cpp\n", argv[0]);
        return 1;
    }

    if (!read_file(argv[1], sourceA, MAX_SOURCE))
        return 1;
    if (!read_file(argv[2], sourceB, MAX_SOURCE))
        return 1;

    allinea(sourceA, sourceB);

    return 0;
}
