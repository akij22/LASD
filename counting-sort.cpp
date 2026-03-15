#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

// File modificato dal counting sort originale del prof
// // Sono state rimosse alcune funzioni inutili
// // L'obiettivo è lanciare l'algoritmo su `data_positive.csv`
// // e verificare i risultati, confrontandoli anche con
// // l'algoritmo finale da implementare

/// intero massimo usato
#define MAX_COUNT 1000000

int ct_swap = 0;
int ct_cmp = 0;
int ct_read = 0; /// conta le operazioni di lettura su array

// Righe del dataset = 100
int ntests = 100;
int ndiv = 1;

int n = 0; /// dimensione dell'array


void print_array(int *A, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}



void counting_sort(int *A, int *B, int *C, int n, int k) {
    /// A: array in input 0..n-1
    /// B: array in output 0..n-1
    /// C: array per conteggi 0..k
    /// n: elementi da ordinare
    /// k: valore massimo contenuto in A

    for (int i = 0; i <= k; i++) { /// reset array conteggi
        C[i] = 0;
    }

    for (int j = 0; j < n; j++) { /// conteggio istogramma
        C[A[j]]++;
        ct_read++; /// lettura A[j]
        ct_read++; /// lettura C[A[j]]
    }

    for (int i = 1; i <= k; i++) { /// C[i] contiene il numero di elementi <= i
        C[i] += C[i - 1];
        ct_read++; /// lettura C[i]
        ct_read++; /// lettura C[i-1]
    }

    for (int j = n - 1; j >= 0; j--) { /// per ogni elemento originale in A ->
                                       /// mi chiedo nel conteggio C quanti sono gli elementi minori o uguali:
                                       /// questo corrisponde alla posizione dell'elemento in B

        B[C[A[j]] - 1] = A[j];
        ct_read++; /// lettura C[A[j]]
        ct_read++; /// lettura A[j] per assegnare in B
        ct_read++; /// lettura A[j] per indicizzare C
        C[A[j]] = C[A[j]] - 1;
        ct_read++; /// lettura C[A[j]] lato destro
        ct_read++; /// lettura A[j] per indicizzare C lato sinistro
        ct_read++; /// lettura A[j] per indicizzare C lato destro
    }

}
/*
int parse_cmd(int argc, char **argv) {
    /// controllo argomenti
    if (argc < 2) {
        printf("Usage: %s max-dim [Options]\n", argv[0]);
        printf("   max-dim: specifica la massima dimensione n del problema\n");
        printf("Options:\n");
        printf("  -d=<int>: Specifica quali dimensioni n del problema vengono lanciate in sequenza [default: 1] \n");
        printf("            n = k * max-dim / d, k=1 .. d\n");
        printf("  -t=<int>: Specifica quanti volte viene lanciato l'algoritmo per una specifica dimensione n [default: 1]\n");
        printf("            Utile nel caso in cui l'input viene inizializzato in modo random\n");
        printf("  -verbose: Abilita stampe durante l'esecuzione dell'algoritmo\n");
        printf("  -graph: creazione file di dot con il grafo dell'esecuzione (forza d=1 t=1)\n");
        return 1;
    }

    /// parsing argomento
    for (int i = 2; i < argc; i++) {
        if (argv[i][1] == 'd')
            ndiv = atoi(argv[i] + 3);
        if (argv[i][1] == 't')
            ntests = atoi(argv[i] + 3);
        if (argv[i][1] == 'v')
            details = 1;
        if (argv[i][1] == 'g') {
            graph = 1;
            ndiv = 1;
            ntests = 1;
        }
    }

    return 0;
}

*/

int main() {
    int i, test;
    int *A;
    int *B;
    int *C;
    int k; /// valore massimo nell'array di input


    /// allocazione array
    A = new int[1000];
    B = new int[1000];
    C = new int[MAX_COUNT]; /// alloco l'array per i conteggi

    ifstream input_data;
    input_data.open("data_positive.csv");
    if (!input_data.is_open()) {
        printf("Errore: impossibile aprire data_positive.csv\n");
        return 1;
    }

    n = 1000;
    int read_min = -1;
    int read_max = -1;
    long read_avg = 0;

    //// lancio ntests volte: ogni test legge la riga successiva del dataset
    for (test = 0; test < ntests; test++) {

            k = 0;

            // Popolazione dell'array dal dataset: ogni test legge la riga successiva
            char line[200000];
            if (!input_data.getline(line, sizeof(line))) {
                printf("Errore lettura riga dataset al test %d\n", test);
                delete[] A;
                delete[] B;
                delete[] C;
                return 1;
            }
            while (line[0] == '\0') {
                if (!input_data.getline(line, sizeof(line))) {
                    printf("Errore lettura riga dataset al test %d\n", test);
                    delete[] A;
                    delete[] B;
                    delete[] C;
                    return 1;
                }
            }

            char *token = strtok(line, ",");
            for (i = 0; i < n; i++) {
                if (token == NULL) {
                    printf("Riga dataset con meno di %d valori (test %d)\n", n, test);
                    delete[] A;
                    delete[] B;
                    delete[] C;
                    return 1;
                }

                A[i] = atoi(token);
                if (k < A[i]) k = A[i]; /// memorizzo il massimo in k
                token = strtok(NULL, ",");
            }

            /// controllo se il massimo valore e' troppo grande
            if (k >= MAX_COUNT) {
                printf("interno in array troppo grande\n");
                return -1;
            }


            ct_read = 0;
            /// algoritmo di sorting
            counting_sort(A, B, C, n, k);

            read_avg += ct_read;
            if (read_min < 0 || read_min > ct_read)
                read_min = ct_read;
            if (read_max < 0 || read_max < ct_read)
                read_max = ct_read;
            printf("Test %d %d\n", test, ct_read);
    }

    printf("N test: %d, Min: %d, Med: %.1f, Max: %d\n",
           ntests,
           read_min,
           (0.0 + read_avg) / ntests,
           read_max);

    // Stampa finale per verificare se l'array finale è ordinato correttamente
    printf("Stampa array finale ordinato:\n");
    print_array(B, n);

    delete[] A;
    delete[] B;
    delete[] C; /// dealloco l'array di supporto per i conteggi (dipende da k = massimo nell'array in uso)

    return 0;
}
