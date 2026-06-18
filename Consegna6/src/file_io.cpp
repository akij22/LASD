#include <stdio.h>

#include "file_io.h"

int read_file(const char *filename, char *buffer, int max_size) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        printf("Errore: impossibile aprire %s\n", filename);
        return 0;
    }

    int nread = fread(buffer, 1, max_size - 1, f);
    buffer[nread] = '\0';
    fclose(f);
    return 1;
}
