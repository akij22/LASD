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

#define NUM_BUCKET 620
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

bool isOrdered(const int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1])
            return false;
    }
    return true;
}

// Segmentazione in 16 intervalli di complessita' piu' contenuta.
int map_to_bucket(int v) {
    if (v <= 939) {
        int b = (v + 1680) / 66;
        if (b < 0)
            b = 0;
        if (b >= 40)
            b = 39;
        return b;
    } else if (v <= 1876) {
        int b = 40 + (v - 940) / 25;
        if (b >= 78)
            b = 77;
        return b;
    } else if (v <= 2803) {
        int b = 78 + (v - 1877) / 24;
        if (b >= 117)
            b = 116;
        return b;
    } else if (v <= 3708) {
        int b = 117 + (v - 2804) / 24;
        if (b >= 156)
            b = 155;
        return b;
    } else if (v <= 4586) {
        int b = 156 + (v - 3709) / 23;
        if (b >= 195)
            b = 194;
        return b;
    } else if (v <= 5430) {
        int b = 195 + (v - 4587) / 22;
        if (b >= 234)
            b = 233;
        return b;
    } else if (v <= 6228) {
        int b = 234 + (v - 5431) / 21;
        if (b >= 273)
            b = 272;
        return b;
    } else if (v <= 6976) {
        int b = 273 + (v - 6229) / 20;
        if (b >= 312)
            b = 311;
        return b;
    } else if (v <= 7667) {
        int b = 312 + (v - 6977) / 19;
        if (b >= 350)
            b = 349;
        return b;
    } else if (v <= 8288) {
        int b = 350 + (v - 7668) / 16;
        if (b >= 389)
            b = 388;
        return b;
    } else if (v <= 8805) {
        int b = 389 + (v - 8289) / 14;
        if (b >= 427)
            b = 426;
        return b;
    } else if (v <= 9218) {
        int b = 427 + (v - 8806) / 11;
        if (b >= 466)
            b = 465;
        return b;
    } else if (v <= 9515) {
        int b = 466 + (v - 9219) / 8;
        if (b >= 505)
            b = 504;
        return b;
    } else if (v <= 9743) {
        int b = 505 + (v - 9516) / 6;
        if (b >= 543)
            b = 542;
        return b;
    } else if (v <= 10037) {
        int b = 543 + (v - 9744) / 8;
        if (b >= 582)
            b = 581;
        return b;
    } else {
        int b = 582 + (v - 10038) / 15;
        if (b >= NUM_BUCKET)
            b = NUM_BUCKET - 1;
        return b;
    }
}

void bucket_sort(int *A, int n, Stats &stats) {

    int buckets[NUM_BUCKET][BUCKET_CAP];
    int bucket_count[NUM_BUCKET];

    for(int i = 0;i < NUM_BUCKET;i++)
        bucket_count[i] = 0;

    for(int i = 0;i < n; i++) {

        int v = A[i];
        stats.ct_read++;

        int b = map_to_bucket(v);

        int count = bucket_count[b];
        stats.ct_read++;

        int pos = count;

        while(pos > 0) {

            int prev = buckets[b][pos - 1];
            stats.ct_read++;
            stats.ct_cmp++;

            if(prev <= v)
                break;

            buckets[b][pos] = prev;
            pos--;
        }

        buckets[b][pos] = v;

        bucket_count[b] = count + 1;
    }

    int k = 0;

    for(int b = 0;b < NUM_BUCKET; b++) {

        int limit = bucket_count[b];
        stats.ct_read++;

        // PASSAGGIO FINALE
        // Copio gli elementi dei soli bucket non vuoti in ordine in A.
        for(int i = 0;i < limit;i++) {

            A[k] = buckets[b][i];
            stats.ct_read++;
            k++;
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

        if (!isOrdered(A, n)) {
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
