#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

// compilazione: g++ bucketSortV2.cpp
//

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

#define NUM_BUCKET 877
int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;

int n = 1000; /// dimensione dell'array
const int MIN_VAL = -1680;
const int MAX_VAL = 10590;
const int BUCKET_RANGE = (MAX_VAL - MIN_VAL) / NUM_BUCKET + 1; // 14

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
    /// aggiorno contatore globale di swap
    ct_swap++;
}

bool isOrdered(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1])
            return false;
    }
    return true;
}

void bucket_sort(int *A, int n) {
    // freq[b][o] = quante volte compare il valore
    // MIN_VAL + b*BUCKET_RANGE + o
    int freq[NUM_BUCKET][BUCKET_RANGE];
    unsigned short usedMask[NUM_BUCKET];

    for (int b = 0; b < NUM_BUCKET; b++) {
        usedMask[b] = 0;
        for (int o = 0; o < BUCKET_RANGE; o++) {
            freq[b][o] = 0;
        }
    }

    // Fase di conteggio.
    // ct_read viene incrementato ad ogni lettura da array.
    for (int i = 0; i < n; i++) {
        int v = A[i];
        ct_read++; // lettura A[i]

        int idx = v - MIN_VAL;
        int b = idx / BUCKET_RANGE;

        if (b < 0)
            b = 0;
        if (b >= NUM_BUCKET)
            b = NUM_BUCKET - 1;

        int off = idx - b * BUCKET_RANGE;
        if (off < 0)
            off = 0;
        if (off >= BUCKET_RANGE)
            off = BUCKET_RANGE - 1;

        int c = freq[b][off];
        ct_read++; // lettura freq[b][off]

        freq[b][off] = c + 1;

        if (c == 0) {

            unsigned short mask = usedMask[b];
            ct_read++; // lettura usedMask[b]
            mask = (unsigned short)(mask | (1u << off));
            usedMask[b] = mask;
        }
    }

    // Ricostruzione ordinata
    int k = 0;
    for (int b = 0; b < NUM_BUCKET; b++) {

        unsigned short mask = usedMask[b];
        ct_read++; // lettura usedMask[b]

        if (mask == 0)
            continue;

        while (mask != 0) {

            // Ricerca manuale del primo bit a 1 (no builtins/STL).
            int off = 0;
            unsigned short tmp = mask;
            while ((tmp & 1u) == 0u) {
                tmp = (unsigned short)(tmp >> 1);
                off++;
            }
            mask = (unsigned short)(mask & (mask - 1));


            int c = freq[b][off];
            ct_read++; // lettura freq[b][off]
            int value = MIN_VAL + b * BUCKET_RANGE + off;

            while (c > 0) {
                A[k] = value; // scrittura
                k++;
                c--;
            }
        }
    }
}

int main() {
    int i, test;
    int *A;

    /// allocazione array
    A = new int[max_dim];

    n = max_dim;


    ifstream input_data;
    input_data.open("datasets/data.csv");

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    //// lancio ntests volte per coprire diversi casi di input random
    for (test = 0; test < ntests; test++) {

        /// inizializzazione array: numeri random con range dimensione array
        for (i = 0; i < n; i++) {
            char comma;
            input_data >> A[i];
            input_data >> comma;
        }

        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;

        /// Lancio l'algoritmo di sorting Bucket Sort
        bucket_sort(A, n);

        /// statistiche
        read_avg += ct_read;
        if (read_min < 0 || read_min > ct_read)
            read_min = ct_read;
        if (read_max < 0 || read_max < ct_read)
            read_max = ct_read;
        printf("Test %d %d\n", test, ct_read);
    }

    printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
           ntests,
           read_min, (0.0 + read_avg) / ntests, read_max);

    printf("Array finale:\n");
    print_array(A, n);

    if (!isOrdered(A, n)) {
        printf("Array non ordinato");
    } else {
        printf("Array e' ordinato");
    }

    delete[] A;

    return 0;
}
