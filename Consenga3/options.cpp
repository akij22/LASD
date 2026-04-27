#include "options.h"

#include <cstdio>
#include <cstring>

ProgramOptions options_default() {
    ProgramOptions options;
    options.verbose = false;
    options.graph = false;
    return options;
}

bool parse_cmd(int argc, char **argv, ProgramOptions *options) {
    int ok_parse = 0;

    for (int i = 1; i < argc; i++) {
        if (std::strlen(argv[i]) > 1 && argv[i][1] == 'v') {
            options->verbose = true;
            ok_parse = 1;
        }

        if (std::strlen(argv[i]) > 1 && argv[i][1] == 'g') {
            options->graph = true;
            ok_parse = 1;
        }
    }

    if (argc > 1 && !ok_parse) {
        std::printf("Usage: %s [Options]\n", argv[0]);
        std::printf("Options:\n");
        std::printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
        std::printf("  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n");
        return false;
    }

    return true;
}
