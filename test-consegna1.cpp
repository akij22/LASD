#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// compilazione: g++ consegna1-loader.cpp
//
// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0;

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;
int details = 0;
int graph = 0;

int n = 0; /// dimensione dell'array

void print_array(int *A, int dim) {
    for (int j = 0; j < dim; j++) {
        printf("%d ", A[j]);
    }
    printf("\n");
}

// Funzione di supporto di swap
void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
    /// aggiorno contatore globale di swap
    ct_swap++;
}

int main(int argc, char **argv) {
    int i, test;
    int *A;
    int *B; /// buffer per visualizzazione algoritmo


    /// allocazione array
    A = new int[max_dim];

    n = max_dim;

    ifstream input_data;
    input_data.open("data.csv");

    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    //// lancio ntests volte per coprire diversi casi di input random
    for (test = 0; test < ntests; test++) {

        /// inizializzazione array: lettura da file
        for (i = 0; i < n; i++) {
            char comma;
            input_data >> A[i];
            input_data >> comma;
        }

        if (details) {
            printf("caricato array di dimensione %d\n", n);
            print_array(A, n);
        }

        ct_swap = 0;
        ct_cmp = 0;
        ct_read = 0;

        /// algoritmo di sorting
        /// radix sort LSD stabile in base 256 (2 passate)
        /// conta SOLO le letture da array:
        /// - leggere A[i], B[i], count[i] costa 1
        /// - variabili locali gratis

        const int OFFSET = 1676; // per gestire i negativi del dataset
        const int BASE = 256;

        B = new int[n];

        // Array di conteggi
        int count[BASE];

        // -------------------------
        // PASSATA 1: byte basso
        // -------------------------

        // Inizializzazione di count a 0
        for (i = 0; i < BASE; i++)
            count[i] = 0;

        // Conteggio occorrenze
        // numero di letture = 1000
        for (i = 0; i < n; i++) {
            int v = A[i];
            ct_read++;

            // Conversione in byte basso
            int key = v + OFFSET;
            int digit = key & 255;

            // Conteggio dei byte bassi nell'apposito "bucket"
            count[digit]++;
        }

        // prefix sums
        // Numero di letture = 255 * 2 = 510
        for (i = 1; i < BASE; i++) {
            int curr = count[i];
            ct_read++;              // lettura count[i]
            int prev = count[i - 1];
            ct_read++;              // lettura count[i-1]
            count[i] = curr + prev;
        }

        // distribuzione stabile in B
        // Numero di letture = 1000 * 2 = 2000
        for (i = n - 1; i >= 0; i--) {
            int v = A[i];
            ct_read++;              // lettura A[i]
            int key = v + OFFSET;
            int digit = key & 255;

            int pos = count[digit];
            ct_read++;              // lettura count[digit]
            pos--;

            B[pos] = v;
            count[digit] = pos;
        }

        // -------------------------
        // PASSATA 2: byte alto
        // -------------------------

        // azzero count
        for (i = 0; i < BASE; i++)
            count[i] = 0;

        // Conteggio occorrenze
        // Numero di letture = 1000
        for (i = 0; i < n; i++) {
            int v = B[i];
            ct_read++;              // lettura B[i]
            int key = v + OFFSET;

            // Conversione in byte alto
            int digit = (key >> 8) & 255;
            count[digit]++;
        }

        // prefix sums
        // Numero di letture = 255 * 2 = 510
        for (i = 1; i < BASE; i++) {
            int curr = count[i];
            ct_read++;              // lettura count[i]
            int prev = count[i - 1];
            ct_read++;              // lettura count[i-1]
            count[i] = curr + prev;
        }

        // distribuzione stabile finale in A
        // Numero di letture = 1000 * 2 = 2000
        for (i = n - 1; i >= 0; i--) {
            int v = B[i];
            ct_read++;              // lettura B[i]
            int key = v + OFFSET;
            int digit = (key >> 8) & 255;

            int pos = count[digit];
            ct_read++;              // lettura count[digit]
            pos--;

            A[pos] = v;
            count[digit] = pos;
        }

        delete[] B;

        if (details) {
            printf("Output:\n");
            print_array(A, n);
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

    printf("Stampa array finale ordinato: ");
    print_array(A, n);


    delete[] A;

    return 0;
}
