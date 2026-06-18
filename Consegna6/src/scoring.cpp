#include <stdlib.h>
#include <string.h>

#include "../include/scoring.h"

// Calcolo della Longest Common Subsequence
int lcs_len(const char *a, const char *b) {
    int la = strlen(a);
    int lb = strlen(b);
    int L[64][64];

    if (la > 63)
        la = 63;
    if (lb > 63)
        lb = 63;

    for (int i = 0; i <= la; i++)
        L[i][0] = 0;
    for (int j = 0; j <= lb; j++)
        L[0][j] = 0;

    for (int i = 1; i <= la; i++)
        for (int j = 1; j <= lb; j++)
            if (a[i - 1] == b[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else if (L[i - 1][j] > L[i][j - 1])
                L[i][j] = L[i - 1][j];
            else
                L[i][j] = L[i][j - 1];

    return L[la][lb];
}


// Dato un singolo token, restituisce l'importanza di esso in termini di punteggio
int token_importance(CodeToken t) {
    if (t.kind == TK_BLOCK_OPEN || t.kind == TK_BLOCK_CLOSE)
        return 18;
    if (strcmp(t.text, "(") == 0 || strcmp(t.text, ")") == 0 ||
        strcmp(t.text, "[") == 0 || strcmp(t.text, "]") == 0)
        return 10;
    if (strcmp(t.text, ";") == 0 || strcmp(t.text, ",") == 0 || t.kind == TK_KEYWORD)
        return 8;
    if (t.kind == TK_NUMBER || t.kind == TK_STRING)
        return 7;
    return 5;
}

int gap_penalty(const CodeToken &t) {
    return -token_importance(t) - t.depth;
}


// Funzione di scoring personalizzata
int token_score(CodeToken a, CodeToken b) {

    // Calcolo il bonus per la profondità di indentazione da applicare insieme allo score
    int depth_bonus = a.depth == b.depth ? 3 : -abs(a.depth - b.depth);


    // Se ho due blocchi di codice totalmente uguali, restituisco uno score alto
    if (strcmp(a.text, b.text) == 0)
        return token_importance(a) + 12 + depth_bonus;

    if (a.kind != b.kind) {

        // Penalizzo differenze di tipo di token
        if ((a.kind == TK_IDENT && b.kind == TK_NUMBER) ||
            (a.kind == TK_NUMBER && b.kind == TK_IDENT))
            return -30;


        if (a.kind == TK_BLOCK_OPEN || a.kind == TK_BLOCK_CLOSE ||
            b.kind == TK_BLOCK_OPEN || b.kind == TK_BLOCK_CLOSE)
            return -25;
        return -10;
    }

    // Le prossime casistiche implicano quindi che `a.kind == b.kind` (stesso tipo di Token)


    // Casistica in cui è avvenuta una rinomina
    // Valuto la Longest Common Subsequence e la profondità in cui si trovano i due token
    // (una rinomina può anche ottenere uno scoring positivo)
    if (a.kind == TK_IDENT) {

        // Calcolo LCS
        int common = lcs_len(a.text, b.text);

        int len_a = strlen(a.text);
        int len_b = strlen(b.text);
        int max_len = len_a > len_b ? len_a : len_b;
        if (max_len == 0)
            max_len = 1;

        return 7 + depth_bonus + (common * 8) / max_len;
    }

    if (a.kind == TK_NUMBER || a.kind == TK_STRING)
        return -12;

    if (a.kind == TK_KEYWORD)
        return -8;

    return -5 + depth_bonus;
}
