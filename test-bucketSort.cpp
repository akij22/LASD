#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// compilazione: g++ test-bucketSort.cpp
//

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

#define NUM_BUCKET 877
#define BUCKET_CAP 35
int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;
int details = 0;
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

void bucket_sort(int *A, int n) {

    int buckets[NUM_BUCKET][BUCKET_CAP];
    int bucket_count[NUM_BUCKET];

    int nonEmpty[1000];
    int nonEmpty_count = 0;


    // Inizializzazione dei bucket
    for(int i = 0; i < NUM_BUCKET; i++)
        bucket_count[i] = 0;

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

        if (count == 0) {

            // Registro il bucket corrente `b` nell'array `nonEmpty`, in quanto sto effettuando un primo inserimento
            // e, d'ora in poi, conterrà > 0 elementi
            nonEmpty[nonEmpty_count] = b;
            nonEmpty_count++;
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

        // Aumento il numero di elementi nel bucket n. b
        bucket_count[b] = count + 1;
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

        // int j = nonEmpty[b];

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
        printf("Array non ordinato!\n");
    } else {
        printf("Array e' ordinato!");
    }

    delete[] A;

    return 0;
}
