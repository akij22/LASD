#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

// compilazione: g++ bucketSortV1.cpp
//

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

#define NUM_BUCKET 614
#define BUCKET_CAP 35

// Mappatura non uniforme dei bucket:
// piu' bucket nelle zone dense del dataset, meno bucket nelle zone sparse.
const int DATA_MIN = -1680;
const int S1_MAX = 366;
const int S2_MAX = 2409;
const int S3_MAX = 4452;
const int S4_MAX = 6495;
const int S5_MAX = 8538;
const int DATA_MAX = 10590;

const int B1 = 27;
const int B2 = 66;
const int B3 = 79;
const int B4 = 104;
const int B5 = 124;
const int B6 = 214;

const int O1 = 0;
const int O2 = O1 + B1;
const int O3 = O2 + B2;
const int O4 = O3 + B3;
const int O5 = O4 + B4;
const int O6 = O5 + B5;

const int R1 = (S1_MAX - DATA_MIN) / B1 + 1;
const int R2 = (S2_MAX - (S1_MAX + 1)) / B2 + 1;
const int R3 = (S3_MAX - (S2_MAX + 1)) / B3 + 1;
const int R4 = (S4_MAX - (S3_MAX + 1)) / B4 + 1;
const int R5 = (S5_MAX - (S4_MAX + 1)) / B5 + 1;
const int R6 = (DATA_MAX - (S5_MAX + 1)) / B6 + 1;


int max_dim = 1000;
int ntests = 100;
int ndiv = 1;
// details = 0: nessun debug
// details = 1: statistiche bucket aggregate dopo tutti i test
// details >= 2: include anche il log per ogni inserimento elemento->bucket
int details = 1;
int n = 1000; /// dimensione dell'array

struct Stats {
    int ct_swap = 0;
    int ct_cmp = 0;
    int ct_read = 0;

    // Inizializzazione del costruttore per Stats
    Stats() {
        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;
    }
};

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

int map_to_bucket(int v) {
    int b;

    if(v <= S1_MAX)
        b = O1 + (v - DATA_MIN) / R1;
    else if(v <= S2_MAX)
        b = O2 + (v - (S1_MAX + 1)) / R2;
    else if(v <= S3_MAX)
        b = O3 + (v - (S2_MAX + 1)) / R3;
    else if(v <= S4_MAX)
        b = O4 + (v - (S3_MAX + 1)) / R4;
    else if(v <= S5_MAX)
        b = O5 + (v - (S4_MAX + 1)) / R5;
    else
        b = O6 + (v - (S5_MAX + 1)) / R6;

    if(b < 0)
        b = 0;
    if(b >= NUM_BUCKET)
        b = NUM_BUCKET - 1;

    // printf("Elemento %d mappato al bucket %d\n", v, b);
    return b;
}

void bucket_sort(int *A, int n, int *out_bucket_count, bool *out_use_precount, Stats &stats) {
    int buckets[NUM_BUCKET][BUCKET_CAP];

    // Numero di elementi per ogni bucket
    int bucket_count[NUM_BUCKET];

    bool use_precount[NUM_BUCKET];

    // Lista di indici dei bucket non vuoti
    int used_buckets[NUM_BUCKET];

    // Quanti bucket non vuoti ci sono
    int used_count = 0;


    // Inizializzazione dei bucket
    for(int i = 0; i < NUM_BUCKET; i++) {
        bucket_count[i] = 0;
        use_precount[i] = false;
    }

    for(int i = 0; i < n; i++) {
        int v = A[i];

        stats.ct_read++;
        // ct_read++;

        // calcolo dell'indice del bucket
        int b = map_to_bucket(v);


        // Ottengo il numero di elementi presenti nel bucket n. b
        int count = bucket_count[b];
        stats.ct_read++;
        // ct_read++;

        // Salva l'indice del bucket quando passa da vuoto a non vuoto
        if(count == 0) {
            // Memorizzo il bucket n. b nella lista di bucket non vuoti
            used_buckets[used_count] = b;
            used_count++;
        }

        // Inserimento ordinato con ricerca binaria
        // Ciò assicura che in ogni bucket i valori siano ordinati
        if(count < BUCKET_CAP) {
            int *bucket = buckets[b];

            int lo = 0;
            int hi = count;

            while(lo < hi) {

                // Calcolo punto medio `mid`
                int mid = lo + (hi - lo) / 2;
                int mv = bucket[mid];
                // ct_read++;

                stats.ct_read++;
                stats.ct_cmp++;
                // ct_cmp++;

                if(mv <= v)
                    lo = mid + 1;
                else
                    hi = mid;
            }

            // Posizione finale in cui inserire il nuovo elemento
            int pos = lo;

            // Sposta in blocco gli elementi a destra della posizione di inserimento di uno
            if(pos < count) {
                memmove(bucket + pos + 1,
                        bucket + pos,
                        (size_t)(count - pos) * sizeof(int));
            }

            // Inserisco il nuovo elemento nella posizione corretta
            bucket[pos] = v;

            // Incremento il numero di elementi presenti nel bucket corrente
            bucket_count[b] = count + 1;
        }
    }

    for(int b = 0; b < NUM_BUCKET; b++) {
        out_bucket_count[b] = bucket_count[b];
        out_use_precount[b] = use_precount[b];
    }

    // I bucket usati vengono ordinati per indice cosi' il merge finale resta crescente
    for(int i = 1; i < used_count; i++) {
        int key = used_buckets[i];
        stats.ct_read++;

        int j = i - 1;

        while(j >= 0 && used_buckets[j] > key) {
            used_buckets[j + 1] = used_buckets[j];
            stats.ct_read++;
            j--;
        }

        used_buckets[j + 1] = key;
    }


    // PASSAGGIO FINALE
    // Copio gli elementi dei soli bucket non vuoti in ordine in A.
    int k = 0;
    for(int idx = 0; idx < used_count; idx++) {
        int b = used_buckets[idx];

        stats.ct_read++;

        // Numero di elementi nel bucket n. b (che non è vuoto, dato che l'ho ottenuto da `used_buckets`)
        int limit = bucket_count[b];
        // ct_read++;
        stats.ct_read++;

        int *bucket = buckets[b];

        for(int i = 0; i < limit; i++) {
            A[k] = bucket[i];
            // ct_read++;
            stats.ct_read++;
            k++;
        }
    }
}

int main() {
    int i, test;
    int *A;
    int *B;
    int test_bucket_count[NUM_BUCKET];
    bool test_use_precount[NUM_BUCKET];
    long long bucket_sum[NUM_BUCKET];
    int bucket_min[NUM_BUCKET];
    int bucket_max[NUM_BUCKET];
    int bucket_nonempty_tests[NUM_BUCKET];
    int bucket_precount_tests[NUM_BUCKET];

    Stats stats;

    /// allocazione array
    A = new int[max_dim];

    n = max_dim;


    ifstream input_data;
    input_data.open("datasets/data.csv");

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    for(i = 0; i < NUM_BUCKET; i++) {
        bucket_sum[i] = 0;
        bucket_min[i] = -1;
        bucket_max[i] = 0;
        bucket_nonempty_tests[i] = 0;
        bucket_precount_tests[i] = 0;
    }

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

        /// Lancio l'algoritmo di sorting Bucket Sort
        bucket_sort(A, n, test_bucket_count, test_use_precount, stats);


        // Controllo che per ogni test l'array finale sia ordinato
        if (!isOrdered(A, n)) {
            printf("Array non e' ordinato...");
            return 1;
        }


        for(i = 0; i < NUM_BUCKET; i++) {
            int c = test_bucket_count[i];
            bucket_sum[i] += c;
            if(bucket_min[i] < 0 || c < bucket_min[i])
                bucket_min[i] = c;
            if(c > bucket_max[i])
                bucket_max[i] = c;
            if(c > 0)
                bucket_nonempty_tests[i]++;
            if(test_use_precount[i])
                bucket_precount_tests[i]++;
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

    if(details >= 1) {
        printf("[DBG] Bucket stats aggregated over %d tests:\n", ntests);
        for(i = 0; i < NUM_BUCKET; i++) {
            printf("[DBG] bucket[%d] min=%d avg=%.2f max=%d non_empty=%d/%d precount=%d/%d\n",
                   i,
                   bucket_min[i],
                   (0.0 + bucket_sum[i]) / ntests,
                   bucket_max[i],
                   bucket_nonempty_tests[i], ntests,
                   bucket_precount_tests[i], ntests);
        }
    }


    delete[] A;

    return 0;
}
