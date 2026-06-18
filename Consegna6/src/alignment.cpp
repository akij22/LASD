#include <stdio.h>
#include <string.h>

#include "../include/alignment.h"
#include "../include/scoring.h"
#include "../include/state.h"
#include "../include/tokenizer.h"

int needleman_wunsch_code(CodeToken A[], int na, CodeToken B[], int nb) {
    M_code[0][0] = 0;
    P_code[0][0] = 0;


    // Inizializzo la prima colonna (i token di A con 0 token di B) --> tutte cancellazioni ("-")
    for (int i = 1; i <= na; i++) {
        M_code[i][0] = M_code[i - 1][0] + gap_penalty(A[i - 1]);
        P_code[i][0] = '-';
    }

    // Inizializzo la prima riga (0 token di A con i token di B) --> tutti inserimenti ("+")
    for (int j = 1; j <= nb; j++) {
        M_code[0][j] = M_code[0][j - 1] + gap_penalty(B[j - 1]);
        P_code[0][j] = '+';
    }


    for (int i = 1; i <= na; i++) {
        for (int j = 1; j <= nb; j++) {

            // 1a possibilità  = diagonale(match oppure mismatch ma che conviene comunque allineare)
            // Invoco la funzione dello score perchè sto confrontando 2 token
            int diag = M_code[i - 1][j - 1] + token_score(A[i - 1], B[j - 1]);

            // 2a possibilità  = sopra (cancellazione nel primo sorgente)
            // Invoco la funzione `gap_penalty` per la cancellazione del token A[i - 1] (quindi non devo considerare alcun confronto)
            int up = M_code[i - 1][j] + gap_penalty(A[i - 1]);

            // 3a possibilità  = sinistra (inserimento nel secondo sorgente)
            // Invoco la funzione `gap_penalty` per l'inserimento del token B[j - 1] (anche qui non devo considerare alcun confronto)
            int left = M_code[i][j - 1] + gap_penalty(B[j - 1]);


            // Assumo come caso base che la diagonale sia il miglior scoring
            M_code[i][j] = diag;

            // Controllo se si tratta di match esatto o di "modifica conveniente" e, in base a ciò,
            // scelgo il simbolo da inserire nella matrice P di backtracking
            if (strcmp(A[i - 1].text, B[j - 1].text) == 0)
                P_code[i][j] = '=';
            else
                P_code[i][j] = '~';


            // Se la cancellazione è migliore della scelta attuale, scelgo quella
            if (up > M_code[i][j]) {
                M_code[i][j] = up;
                P_code[i][j] = '-';
            }

            // Se l'inserimento è migliore della scelta attuale, scelgo quella
            if (left > M_code[i][j]) {
                M_code[i][j] = left;
                P_code[i][j] = '+';
            }
        }
    }

    return M_code[na][nb];
}

void print_change_string_rec(CodeToken A[], CodeToken B[], int i, int j) {
    if (i == 0 && j == 0)
        return;

    char op = P_code[i][j];
    if (op == '=' || op == '~') {
        print_change_string_rec(A, B, i - 1, j - 1);
        if (op == '~')
            printf("%s", B[j - 1].text);
    } else if (op == '-') {
        print_change_string_rec(A, B, i - 1, j);
        printf("-");
    } else {
        print_change_string_rec(A, B, i, j - 1);
        printf("%s", B[j - 1].text);
    }
}

void print_actions_rec(CodeToken A[], CodeToken B[], int i, int j) {
    if (i == 0 && j == 0)
        return;

    char op = P_code[i][j];
    if (op == '=') {
        print_actions_rec(A, B, i - 1, j - 1);
        return;
    }

    if (op == '~') {
        print_actions_rec(A, B, i - 1, j - 1);
        printf("~ riga %d -> %d modifica %-12s [%s] in %-12s [%s]\n",
               A[i - 1].line, B[j - 1].line,
               A[i - 1].text, from_kind_to_name(A[i - 1].kind),
               B[j - 1].text, from_kind_to_name(B[j - 1].kind));
        return;
    }

    if (op == '-') {
        print_actions_rec(A, B, i - 1, j);
        printf("- riga %d cancella %-12s [%s]\n",
               A[i - 1].line, A[i - 1].text, from_kind_to_name(A[i - 1].kind));
        return;
    }

    print_actions_rec(A, B, i, j - 1);
    printf("+ riga %d aggiungi  %-12s [%s]\n",
           B[j - 1].line, B[j - 1].text, from_kind_to_name(B[j - 1].kind));
}

int count_lines(const char *text) {
    int lines = 0;
    for (int i = 0; text[i] != '\0'; i++)
        if (text[i] == '\n')
            lines++;
    return lines;
}

void allinea(char *src1, char *src2) {

    // Tokenizzazione dei due codici sorgenti
    int na = tokenize_source_code(src1, tokenA);
    int nb = tokenize_source_code(src2, tokenB);

    // Calcolo dello scoring di allineamento finale
    int score = needleman_wunsch_code(tokenA, na, tokenB, nb);

    printf("Needleman-Wunsch per confronto tra versioni C++\n");
    printf("Versione 1: %d righe, %d token\n", count_lines(src1), na);
    printf("Versione 2: %d righe, %d token\n", count_lines(src2), nb);
    printf("Score allineamento: %d\n\n", score);

    printf("Stringa cambiamento:\n");
    print_change_string_rec(tokenA, tokenB, na, nb);
    printf("\n");

    printf("\nLista azioni principali:\n");
    print_actions_rec(tokenA, tokenB, na, nb);
}
