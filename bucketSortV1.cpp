#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// compilazione: g++ test-bucketSort.cpp
//

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

#define NUM_BUCKET 700
#define BUCKET_CAP 35
#define PRECOUNT_BUCKETS 4
#define PRECOUNT_THRESHOLD 16
int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int ct_read_bucket = 0;

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;
// details = 0: nessun debug
// details = 1: statistiche bucket aggregate dopo tutti i test
// details >= 2: include anche il log per ogni inserimento elemento->bucket
int details = 1;
int graph = 0;

int n = 1000; /// dimensione dell'array

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

void bucket_sort(int *A, int n, int *out_bucket_count, bool *out_use_precount) {
    int buckets[NUM_BUCKET][BUCKET_CAP];
    int bucket_count[NUM_BUCKET];
    bool use_precount[NUM_BUCKET];
    int selected_precount = 0;


    // Inizializzazione dei bucket
    for(int i = 0; i < NUM_BUCKET; i++) {
        bucket_count[i] = 0;
        use_precount[i] = false;
    }

    // Definizione dei valori massimi e minimi analizzati dal dataset `data.csv`
    int MIN_VAL = -1680;
    int MAX_VAL = 10590;

    // Calcolo del range che ciascun bucket può coprire
    int bucket_range = (MAX_VAL - MIN_VAL) / NUM_BUCKET + 1;

    for(int i = 0; i < n; i++) {
        int v = A[i];
        ct_read++;

        // Calcolo del bucket in cui inserire l'elemento corrente
        int b = (v - MIN_VAL) / bucket_range;

        // Controllo per non fuoriuscire dai limiti dell'array `buckets`
        if(b < 0)
            b = 0;

        if(b >= NUM_BUCKET)
            b = NUM_BUCKET - 1;


        // Ottengo il numero di elementi presenti nel bucket n. b
        int count = bucket_count[b];
        ct_read++;

        // Attivo la pre-counting strategy sui primi 4 bucket che ricevono elementi.
        if(count == 0 && selected_precount < PRECOUNT_BUCKETS) {
            use_precount[b] = true;
            selected_precount++;
        }

        // I bucket marcati per pre-counting vengono popolati senza mantenerli ordinati.
        if(use_precount[b]) {
            buckets[b][count] = v;
            bucket_count[b] = count + 1;

            continue;
        }

        // Ottengo l'ultima posizione disponibile nel bucket b
        int pos = count;

        int *bucket = buckets[b];


        // INSERTION SORT all'interno del bucket
        // In modo da mantenere il bucket ordinato
        // Sposto gli elementi > v a destra di esso per fargli spazio
        while(pos > 0) {
            int prev = bucket[pos - 1];
            ct_read++;
            ct_cmp++;

            if(prev <= v)
                break;

            bucket[pos] = prev;
            pos--;
        }

        // Inserisco nel bucket n. b l'elemento v nella posizione corretta
        buckets[b][pos] = v;

        if(details >= 2) {
            printf("[DBG] A[%d]=%d -> bucket[%d], pos=%d (insertion)\n",
                   i, v, b, pos);
        }

        // Aumento il numero di elementi nel bucket n. b
        bucket_count[b] = count + 1;
    }

    for(int b = 0; b < NUM_BUCKET; b++) {
        out_bucket_count[b] = bucket_count[b];
        out_use_precount[b] = use_precount[b];
    }


    // PASSAGGIO FINALE
    // Copio gli elementi dei bucket in ordine in A
    int k = 0;
    for(int b = 0; b < NUM_BUCKET; b++) {

        // Numero di elementi nel bucket n. b
        int limit = bucket_count[b];
        ct_read++;

        // Ottengo un puntatore al bucket n. b,
        // Dove posso scorrere gli elementi di quella specifica "riga"
        // E' equivalente a scrivere `buckets[b][i], buckets[b][i - 1], ...`

        int *bucket = buckets[b];


        if(use_precount[b]) {
            // Se il bucket e' abbastanza grande, usa counting sort locale (range del bucket).
            if(limit >= PRECOUNT_THRESHOLD) {
                int freq[BUCKET_CAP];
                for(int off = 0; off < bucket_range; off++)
                    freq[off] = 0;

                int base = MIN_VAL + b * bucket_range;

                for(int i = 0; i < limit; i++) {
                    int v = bucket[i];
                    ct_read++;

                    int off = v - base;
                    if(off < 0)
                        off = 0;
                    if(off >= bucket_range)
                        off = bucket_range - 1;

                    int c = freq[off];
                    ct_read++;
                    freq[off] = c + 1;
                }

                for(int off = 0; off < bucket_range; off++) {
                    int c = freq[off];
                    ct_read++;
                    int value = base + off;
                    while(c > 0) {
                        A[k] = value;
                        k++;
                        c--;
                    }
                }
            } else {
                // Bucket piccolo: insertion sort locale e poi copia.
                for(int i = 1; i < limit; i++) {
                    int v = bucket[i];
                    ct_read++;
                    int pos = i;

                    while(pos > 0) {
                        int prev = bucket[pos - 1];
                        ct_read++;
                        ct_cmp++;
                        if(prev <= v)
                            break;

                        bucket[pos] = prev;
                        pos--;
                    }
                    bucket[pos] = v;
                }

                for(int i = 0; i < limit; i++) {
                    A[k] = bucket[i];
                    ct_read++;
                    k++;
                }
            }
            continue;
        }


        // FASE FINALE
        // Copio l'elemento in posizione k del bucket n. b in A
        // Ripeto ciò per ogni elemento del bucket, indicato da `bucket_count[b]`
        for(int i = 0; i < limit; i++) {
            A[k] = bucket[i];
            ct_read++;
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

        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;

        /// Lancio l'algoritmo di sorting Bucket Sort
        bucket_sort(A, n, test_bucket_count, test_use_precount);

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
