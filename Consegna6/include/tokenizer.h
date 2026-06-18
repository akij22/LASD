#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "code_token.h"

int is_ident_start(char c);
int is_ident_char(char c);
int is_keyword(const char *s);

// Dato un TokenKind, restituisce il nome del tipo di token corrispondente (sottoforma di stringa)
const char* from_kind_to_name(TokenKind kind);

void copy_token_text(char *dst, const char *src, int len);

// Funzione per scorrere i caratteri di un file sorgente e costruire i token
// Restituisce il numero di token prodotti
int tokenize_source_code(const char *src, CodeToken tokens[]);

#endif
