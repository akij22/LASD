#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "code_token.h"

int needleman_wunsch_code(CodeToken A[], int na, CodeToken B[], int nb);
void print_change_string_rec(CodeToken A[], CodeToken B[], int i, int j);
void print_actions_rec(CodeToken A[], CodeToken B[], int i, int j);
int count_lines(const char *text);
void allinea(char *src1, char *src2);

#endif
