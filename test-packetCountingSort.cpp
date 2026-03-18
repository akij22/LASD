#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// compilazione: g++ consegna1.cpp -o consegna1
//
// Algoritmo: Packed Counting Sort con nibble packing (4 bit per contatore, 16 contatori per long long)
// Sfrutta le proprieta' dei dati: valori interi nel range [-1676, 10575] (range 12252).
// Con 1000 elementi distribuiti su 12252 valori distinti, nessun valore puo' apparire
// piu' di 15 volte, quindi i contatori a 4 bit non saturano mai: nessun overflow handling necessario.
// Letture totali per serie: 2n + ceil(range/16) = 2000 + 766 = 2766

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;

int n = 0;

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

bool isOrdered(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1])
            return false;
    }
    return true;
}


void packed_counting_sort(int *A, int n) {
    const int MIN_VAL = -1676;
    const int RANGE = 12252;           // 10575 - (-1676) + 1
    const int CPW = 16;              // contatori per word (64 bit / 4 bit)
    const int PSIZE = (RANGE + CPW - 1) / CPW; // ceil(12252/16) = 766

    long long packed[PSIZE];
    memset(packed, 0, sizeof(packed));

    // Fase 1: conta le occorrenze di ogni valore (2 letture per elemento)
    for (int i = 0; i < n; i++) {
        int val   = A[i] - MIN_VAL;
        ct_read++;

        int w     = val / CPW;
        int shift = (val % CPW) * 4;

        long long word = packed[w];
        ct_read++;

        packed[w] = (word & ~(0xFLL << shift)) | ((((word >> shift) & 0xFLL) + 1) << shift);
    }

    // Fase 2: ricostruisce A scansionando i contatori (1 lettura per word)
    int pos = 0;
    for (int w = 0; w < PSIZE; w++) {
        long long word = packed[w];
        ct_read++;

        if (word == 0) continue;

        int base = w * CPW + MIN_VAL;
        int lim  = (w == PSIZE - 1) ? RANGE - w * CPW : CPW;

        for (int j = 0; j < lim; j++) {
            int c = (int)((word >> (j * 4)) & 0xFLL);
            while (c--) A[pos++] = base + j;
        }
    }
}


int main(int argc, char **argv) {
    int i, test;
    int *A;

    A = new int[max_dim];
    n = max_dim;

    ifstream input_data;
    input_data.open("datasets/data.csv");

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    for (test = 0; test < ntests; test++) {

        for (i = 0; i < n; i++) {
            char comma;
            input_data >> A[i];
            input_data >> comma;
        }

        ct_swap = 0;
        ct_cmp  = 0;
        ct_read = 0;

        packed_counting_sort(A, n);

        read_avg += ct_read;
        if (read_min < 0 || read_min > ct_read) read_min = ct_read;
        if (read_max < 0 || read_max < ct_read) read_max = ct_read;
        printf("Test %d %d\n", test, ct_read);
    }

    printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
           ntests, read_min, (0.0 + read_avg) / ntests, read_max);


    print_array(A, n);

    if (isOrdered(A, n))
        printf("Array is ordered\n");
    else
        printf("Array is NOT ordered\n");

    delete[] A;
    return 0;
}
