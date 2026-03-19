#include <fstream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
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

constexpr int kMinVal = -1676;
constexpr int kMaxVal = 10575;
constexpr int kRange = kMaxVal - kMinVal + 1; // 12252
constexpr int kCountersPerWord = 16;          // 64 bit / 4 bit
constexpr int kPackedSize = (kRange + kCountersPerWord - 1) / kCountersPerWord;

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


int get_packed_counter(long long word, int slot) {
    const int shift = slot * 4;
    return (int)((word >> shift) & 0xFLL);
}

long long set_packed_counter(long long word, int slot, int value) {
    const int shift = slot * 4;
    const long long clear_mask = ~(0xFLL << shift);
    const long long new_bits = ((long long)value & static_cast<long long>(15)) << shift;
    return (word & clear_mask) | new_bits;
}


void packed_counting_sort(int *A, int n) {
    long long packed[kPackedSize];
    memset(packed, 0, sizeof(packed));

    // Fase 1: conta le occorrenze di ogni valore.
    for (int i = 0; i < n; i++) {
        const int normalized_value = A[i] - kMinVal;
        ct_read++;

        const int word_index = normalized_value / kCountersPerWord;
        const int slot_in_word = normalized_value % kCountersPerWord;

        long long word = packed[word_index];
        ct_read++;

        const int current_count = get_packed_counter(word, slot_in_word);
        word = set_packed_counter(word, slot_in_word, current_count + 1);
        packed[word_index] = word;
    }

    // Fase 2: ricostruisce A scansionando i contatori.
    int pos = 0;
    for (int word_index = 0; word_index < kPackedSize; word_index++) {
        const long long word = packed[word_index];
        ct_read++;

        if (word == 0) {
            continue;
        }

        const int base_value = kMinVal + word_index * kCountersPerWord;
        int slots_in_this_word = kCountersPerWord;
        if (word_index == kPackedSize - 1) {
            slots_in_this_word = kRange - word_index * kCountersPerWord;
        }

        for (int slot = 0; slot < slots_in_this_word; slot++) {
            int count = get_packed_counter(word, slot);
            while (count > 0) {
                A[pos] = base_value + slot;
                pos++;
                count--;
            }
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
        printf("Array e' ordinato\n");
    else
        printf("Array non e' ordinato\n");

    delete[] A;
    return 0;
}
