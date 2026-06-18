#include <ctype.h>
#include <string.h>

#include "../include/tokenizer.h"

int is_ident_start(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

int is_ident_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int is_keyword(const char *s) {
    const char *keywords[] = {
        "if", "else", "for", "while", "do", "switch", "case", "break",
        "continue", "return", "class", "struct", "public", "private",
        "protected", "const", "void", "int", "double", "float", "bool",
        "char", "string", "vector", "map", "include", "using", "namespace",
        "new", "delete", "true", "false"
    };
    int n_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n_keywords; i++)
        if (strcmp(s, keywords[i]) == 0)
            return 1;
    return 0;
}


// Dato un TokenKind, restituisce il nome del tipo di token corrispondente (sottoforma di stringa)
const char* from_kind_to_name(TokenKind kind) {
    switch (kind) {
    case TK_IDENT: return "ident";
    case TK_KEYWORD: return "keyword";
    case TK_NUMBER: return "number";
    case TK_STRING: return "string";
    case TK_SYMBOL: return "symbol";
    case TK_BLOCK_OPEN: return "block-open";
    case TK_BLOCK_CLOSE: return "block-close";
    case TK_OPERATOR: return "operator";
    }
    return "?";
}

void copy_token_text(char *dst, const char *src, int len) {
    if (len > MAX_TEXT)
        len = MAX_TEXT;
    for (int i = 0; i < len; i++)
        dst[i] = src[i];
    dst[len] = '\0';
}


// Funzione per scorrere i caratteri di un file sorgente e costruire i token
// Restituisce il numero di token prodotti
int tokenize_source_code(const char *src, CodeToken tokens[]) {
    int line = 1;
    int depth = 0;
    int ntokens = 0;

    for (int i = 0; src[i] != '\0' && ntokens < MAX_TOKEN;) {
        char c = src[i];


        // Se il carattere attuale è uno spazio / tab, non lo considero
        if (isspace((unsigned char)c)) {
            if (c == '\n')
                line++;
            i++;
            continue;
        }

        // Ignoro i commenti (della forma // o /**/)
        if (c == '/' && src[i + 1] == '/') {
            i += 2;
            while (src[i] != '\0' && src[i] != '\n')
                i++;
            continue;
        }

        if (c == '/' && src[i + 1] == '*') {
            i += 2;
            while (src[i] != '\0' && !(src[i] == '*' && src[i + 1] == '/')) {
                if (src[i] == '\n')
                    line++;
                i++;
            }
            if (src[i] != '\0')
                i += 2;
            continue;
        }


        // Identificatore / keyword
        if (is_ident_start(c)) {
            int start = i;
            while (is_ident_char(src[i]))
                i++;

            // Creazione del token ident/keyword
            CodeToken t;
            copy_token_text(t.text, src + start, i - start);

            // Valuto se il Token è di tipo keyword o identificatore
            t.kind = is_keyword(t.text) ? TK_KEYWORD : TK_IDENT;
            t.line = line;
            t.depth = depth;
            tokens[ntokens++] = t;
            continue;
        }

        // Numero
        if (isdigit((unsigned char)c)) {
            int start = i;
            while (isalnum((unsigned char)src[i]) || src[i] == '.')
                i++;

            // Creazione del token number
            CodeToken t;
            copy_token_text(t.text, src + start, i - start);
            t.kind = TK_NUMBER;
            t.line = line;
            t.depth = depth;
            tokens[ntokens++] = t;
            continue;
        }

        // Stringa
        if (c == '"' || c == '\'') {
            char quote = c;
            int start = i++;
            while (src[i] != '\0') {
                if (src[i] == '\n')
                    line++;

                if (src[i] == '\\' && src[i + 1] != '\0') {
                    i += 2;
                    continue;
                }

                if (src[i++] == quote)
                    break;
            }

            // Creazione del token string
            CodeToken t;
            copy_token_text(t.text, src + start, i - start);
            t.kind = TK_STRING;
            t.line = line;
            t.depth = depth;
            tokens[ntokens++] = t;
            continue;
        }


        CodeToken t;
        t.text[0] = c;
        t.text[1] = '\0';
        t.line = line;
        if (c == '{') {

            t.kind = TK_BLOCK_OPEN;
            t.depth = depth;
            depth++;

        } else if (c == '}') {
            if (depth > 0)
                depth--;
            t.kind = TK_BLOCK_CLOSE;
            t.depth = depth;

        } else if (c == '(' || c == ')' || c == '[' || c == ']' || c == ';' || c == ',') {
            t.kind = TK_SYMBOL;
            t.depth = depth;

        } else {
            if ((c == '=' && src[i + 1] == '=') || (c == '!' && src[i + 1] == '=') ||
                (c == '<' && src[i + 1] == '=') || (c == '>' && src[i + 1] == '=') ||
                (c == '+' && src[i + 1] == '+') || (c == '-' && src[i + 1] == '-') ||
                (c == '+' && src[i + 1] == '=') || (c == '-' && src[i + 1] == '=') ||
                (c == '&' && src[i + 1] == '&') || (c == '|' && src[i + 1] == '|')) {
                t.text[0] = c;
                t.text[1] = src[i + 1];
                t.text[2] = '\0';
                i++;
            }
            t.kind = TK_OPERATOR;
            t.depth = depth;
        }
        tokens[ntokens++] = t;
        i++;
    }
    return ntokens;
}
