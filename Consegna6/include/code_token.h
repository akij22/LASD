#ifndef CODE_TOKEN_H
#define CODE_TOKEN_H

/* Crea un insieme di possibili token da accettare
 * TK_IDENT = nomi scelti come identificatori di variabili / strutture dati
 * TK_KEYWORD = parole chiave del linguaggio (if, else, for, ...)
 * TK_NUMBER = numeri
 * TK_STRING = stringhe
 * TK_SYMBOL = simboli "semplici"
 * TK_BLOCK_OPEN = apertura del blocco {
 * TK_BLOCK_CLOSE = chiusura del blocco }
 * TK_OPERATOR = operatori di aritmetica / confronto (+, -, *, /, ...)
 */
enum TokenKind {
    TK_IDENT,
    TK_KEYWORD,
    TK_NUMBER,
    TK_STRING,
    TK_SYMBOL,
    TK_BLOCK_OPEN,
    TK_BLOCK_CLOSE,
    TK_OPERATOR
};


// Definizione della struttura Token
// IDEA: non confronto sinoli caratteri, ma blocchi di codice
typedef struct CodeToken {
    char text[64];

    // Tipo di token
    TokenKind kind;

    // Linea di codice
    int line;

    // Profondità di indentazione (influisce sullo scoring finale)
    int depth;
} CodeToken;

const int MAX_TOKEN = 1200;
const int MAX_SOURCE = 60000;
const int MAX_TEXT = 63;

#endif
