#ifndef SCORING_H
#define SCORING_H

#include "code_token.h"

// Calcolo della Longest Common Subsequence
int lcs_len(const char *a, const char *b);

// Dato un singolo token, restituisce l'importanza di esso in termini di punteggio
int token_importance(CodeToken t);
int gap_penalty(const CodeToken &t);

// Funzione di scoring personalizzata
int token_score(CodeToken a, CodeToken b);

#endif
