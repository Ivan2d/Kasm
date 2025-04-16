#pragma once
#include "types.h"

extern int token_count;

extern char *KEY2[];
extern char *KEYR[];

#define KEY2_SIZE 46
#define KEYR_SIZE 8

Token *Lex(const char *prog, const char *filename);
