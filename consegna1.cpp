#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

// GRUPPO DI LAVORO composto da Achille Rossi, Pietro Dondi, Filippo Miari

// compilazione: g++ consegna1.cpp

// Il programma carica il file data.csv contenente 100 righe con dati da ordinare in modo crescente
// ./a.out
// In output viene mostrato il numero di accessi in read alla memoria per eseguire il sorting di ciascuna riga

// Obiettivo:
// Creare un algoritmo di sorting che minimizzi la somma del numero di accessi per ogni sorting di ciascuna riga del file

// Commento che spiega le parti generate dall'LLM
/*
    Lo spunto per trovare un algoritmo efficace per ordinare il dataset data.csv ci è venuto analizzando la disposizione dei dati.
    I 100 sets di numeri hanno in comune un andamento sinusoidale, con la prima metà dei numeri divisa in "blocchi" da 30 elementi circa molto simili tra loro.
    Da metà in poi i numeri sono più casuali, senza seguire un pattern comune.
    Sfruttando questa proprietà l'utilizzo di Buckets è ottimale.

    L’algoritmo sviluppato è una variante del bucket sort pensata per ridurre il numero di letture in memoria.
    L’idea di base è quella di suddividere i dati in molti piccoli gruppi (bucket)
    utilizzando una funzione di mapping costruita per distribuire correttamente i valori.
    In questo modo ogni bucket contiene pochi elementi.
    Durante l’inserimento, ogni elemento viene posizionato direttamente nel bucket corretto mantenendo l’ordine
    tramite semplici confronti tra elementi.
    Poiché i bucket sono molto piccoli, gli spostamenti e i confronti necessari risultano limitati,
    evitando di dover rileggere l’intero array ogni volta.

    Alla fine, i bucket vengono concatenati perché contengono dati ordinati
    e sono già organizzati tra loro tramite la funzione di mapping.

    Questo approccio permette di lavorare sempre su sottoinsiemi ridotti di dati,
    riducendo significativamente il numero totale di letture rispetto a un algoritmo come quicksort.

    Durante lo sviluppo sono state testate anche altre soluzioni,
    come l’uso di binary search e strutture circolari (ring buffer) per ottimizzare l’inserimento,
    ma queste introducevano un numero maggiore di letture. La soluzione finale a bucket è risultata quindi la più efficace.

    */

#define NUM_BUCKET 614
#define BUCKET_CAP 35

int max_dim = 1000;
int ntests = 100;
int ndiv = 1;

int n = 0;

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

// Funzione di supporto per mappare un bucket all'interno di un intervallo specifico.
// Garantisce che l'elemento fornito in input  sia all'interno dell'intervallo [lo, hi]
int support_map(int b, int lo, int hi) {
    if (b < lo) return lo;
    if (b > hi) return hi;
    return b;
}

// Segmentazione in intervalli non lineare
// Se fosse lineare, avverrebbe uno squilibrio nei bucket, con alcuni di essi molti pieni e altri quasi vuoti, causando un "overhead" nell'ordinamento di ogni bucket
// // La scelta di un ordinamento non lineare è una diretta conseguenza della distribuzione dei dati nel dataset fornito
int map_to_bucket(int v) {

    if (v <= 939) {

        // Mappatura dal bucket 0 a 39, dove ogni bucket corrisponde a 66 valori consecutivi
        return support_map((v + 1680) / 66, 0, 39);
    } else if (v <= 1876) {

        // Mappatura dal bucket 40 a 77, dove ogni bucket corrisponde a 25 valori consecutivi
        return support_map(40 + (v - 940) / 25, 40, 77);
    } else if (v <= 2803) {

        // Mappatura dal bucket 78 a 116, dove ogni bucket corrisponde a 24 valori consecutivi
        return support_map(78 + (v - 1877) / 24, 78, 116);
    } else if (v <= 3708) {

        // Mappatura dal bucket 117 a 155, dove ogni bucket corrisponde a 24 valori consecutivi
        return support_map(117 + (v - 2804) / 24, 117, 155);
    } else if (v <= 4586) {

        // Mappatura dal bucket 156 a 194, dove ogni bucket corrisponde a 23 valori consecutivi
        return support_map(156 + (v - 3709) / 23, 156, 194);
    } else if (v <= 5430) {

        // Mappatura dal bucket 195 a 233, dove ogni bucket corrisponde a 22 valori consecutivi
        return support_map(195 + (v - 4587) / 22, 195, 233);
    } else if (v <= 6228) {

        // Mappatura dal bucket 234 a 272, dove ogni bucket corrisponde a 21 valori consecutivi
        return support_map(234 + (v - 5431) / 21, 234, 272);
    } else if (v <= 6976) {

        // Mappatura dal bucket 273 a 311, dove ogni bucket corrisponde a 20 valori consecutivi
        return support_map(273 + (v - 6229) / 20, 273, 311);
    } else if (v <= 7667) {

        // Mappatura dal bucket 312 a 349, dove ogni bucket corrisponde a 19 valori consecutivi
        return support_map(312 + (v - 6977) / 19, 312, 349);
    } else if (v <= 8288) {

        // Mappatura dal bucket 350 a 388, dove ogni bucket corrisponde a 16 valori consecutivi
        return support_map(350 + (v - 7668) / 16, 350, 388);
    } else if (v <= 8805) {

        // Mappatura dal bucket 389 a 426, dove ogni bucket corrisponde a 14 valori consecutivi
        return support_map(389 + (v - 8289) / 14, 389, 426);
    } else if (v <= 9218) {

        // Mappatura dal bucket 427 a 465, dove ogni bucket corrisponde a 11 valori consecutivi
        return support_map(427 + (v - 8806) / 11, 427, 465);
    } else if (v <= 9515) {

        // Mappatura dal bucket 466 a 504, dove ogni bucket corrisponde a 8 valori consecutivi
        return support_map(466 + (v - 9219) / 8, 466, 504);
    } else if (v <= 9743) {

        // Mappatura dal bucket 505 a 542, dove ogni bucket corrisponde a 6 valori consecutivi
        return support_map(505 + (v - 9516) / 6, 505, 542);
    } else if (v <= 10037) {

        // Mappatura dal bucket 543 a 581, dove ogni bucket corrisponde a 8 valori consecutivi
        return support_map(543 + (v - 9744) / 8, 543, 581);
    } else {

        // Mappatura dal bucket 582 a NUM_BUCKET - 1, dove ogni bucket corrisponde a 15 valori consecutivi
        return support_map(582 + (v - 10038) / 15, 582, NUM_BUCKET - 1);
    }
}

// Inserimento locale stile gnome sort: adatto a bucket con pochi elementi.
void insert_bucket_gnome(int *bucket, int count, int value, Stats &stats) {
    bucket[count] = value;
    int pos = count;

    while (pos > 0) {
        int left = bucket[pos - 1];
        stats.ct_read++;
        stats.ct_cmp++;

        if (left <= value)
            break;

        bucket[pos] = left;
        pos--;
        stats.ct_swap++;
    }
    bucket[pos] = value;
}

void bucket_sort(int *A, int n, Stats &stats) {

    int buckets[NUM_BUCKET][BUCKET_CAP];
    int bucket_count[NUM_BUCKET];

    // Inizializzazione del contatore di elementi per ogni bucket
    for(int i = 0; i < NUM_BUCKET; i++)
        bucket_count[i] = 0;


    // Scorro tutti gli elementi dell'array originale
    for(int i = 0; i < n; i++) {

        int v = A[i];
        stats.ct_read++;

        // Per ogni elemento, mappiamo il bucket corrispondente utilizzando la funzione `map_to_bucket`
        int b = map_to_bucket(v);

        // Ottengo quanti elementi sono già presenti nel bucket
        int count = bucket_count[b];
        stats.ct_read++;

        // Inserimento locale con gnome sort.
        insert_bucket_gnome(buckets[b], count, v, stats);
        bucket_count[b] = count + 1;
    }


    int k = 0;
    // PASSAGGIO FINALE
    // Copio gli elementi dei soli bucket non vuoti in ordine in A.
    for(int b = 0; b < NUM_BUCKET; b++) {

        int limit = bucket_count[b];
        stats.ct_read++;

        for(int i = 0; i < limit; i++) {

            A[k] = buckets[b][i];
            stats.ct_read++;
            k++;
        }
    }
}


int main() {
    int i, test;
    int *A;
    Stats stats;

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

        // Statistiche
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
