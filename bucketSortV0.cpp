#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

// compilazione: g++ consegna1-loader.c
//

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

#define NUM_BUCKET 877
#define BUCKET_CAP 35

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;

int n = 0; /// dimensione dell'array

struct Stats {
    int ct_swap = 0;
    int ct_cmp = 0;
    int ct_read = 0;
};

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

bool isOrdered(int *A, int n, Stats &stats) {
    for (int i = 1; i < n; i++) {
        // stats.ct_read += 2;
        if (A[i] < A[i - 1])
            return false;
    }
    return true;
}

void bucket_sort(int *A, int n, Stats &stats) {

    int buckets[NUM_BUCKET][BUCKET_CAP];
    int bucket_count[NUM_BUCKET];

    for(int i=0;i<NUM_BUCKET;i++)
        bucket_count[i] = 0;

    int MIN_VAL = -1680;
    int MAX_VAL = 10590;

    int bucket_size = (MAX_VAL - MIN_VAL) / NUM_BUCKET + 1;

    for(int i = 0;i < n;i++) {

        stats.ct_read++;
        int v = A[i];

        int b = (v - MIN_VAL) / bucket_size;

        if(b < 0) b = 0;
        if(b >= NUM_BUCKET) b = NUM_BUCKET-1;


        int count = bucket_count[b];
        stats.ct_read++;

        int pos = count;

        int *bucket = buckets[b];

        while(pos > 0) {


            int prev = bucket[pos-1];
            stats.ct_read++;
            stats.ct_cmp++;

            if(prev <= v)
                break;

            bucket[pos] = prev;
            pos--;
        }

        buckets[b][pos] = v;

        bucket_count[b] = count + 1;
    }

    int k = 0;

    for(int b = 0;b < NUM_BUCKET;b++) {


        int limit = bucket_count[b];
        stats.ct_read++;

        int *bucket = buckets[b];

        // PASSAGGIO FINALE
        // Copio gli elementi dei soli bucket non vuoti in ordine in A.
        for(int i = 0;i < limit;i++) {

            A[k++] = bucket[i];
            stats.ct_read++;
        }
    }
}


int main(int argc, char **argv) {
    int i, test;
    int *A;
    Stats stats;

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

        stats.ct_swap = 0;
        stats.ct_cmp = 0;
        stats.ct_read = 0;

        /// algoritmo di sorting
        bucket_sort(A, n, stats);

        if (!isOrdered(A, n, stats)) {
            printf("Array non e' ordinato...");
            delete[] A;
            return 1;
        }

        /// statistiche
        read_avg += stats.ct_read;
        if (read_min < 0 || read_min > stats.ct_read)
            read_min = stats.ct_read;
        if (read_max < 0 || read_max < stats.ct_read)
            read_max = stats.ct_read;
        printf("Test %d %d\n", test, stats.ct_read);
    }

    printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
           ntests,
           read_min, (0.0 + read_avg) / ntests, read_max);

    delete[] A;

    return 0;
}
