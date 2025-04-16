#pragma once
#include "lexer.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *HUMAN_TOKS[];
extern const char *HUMAN_AST[];

#define HUMAN_TOKS_SIZE 14
#define HUMAN_AST_SIZE 5

void PrintTokens(Token toks[], int count);
void PrintAst(int toks[][4], int count);
void PrintLabs(Label labs[], int count);
void ExportLabs(const char *filename, Label labs[], int count);
int ImportLabs(const char *filename, Label labs[], int max_count);
void OffsetLabs(Label labs[], int count, int offset);
void Usage();
void freeTokens(Token *tokens);
void AsWord(uint16_t value, uint8_t *low, uint8_t *high);
void As24(uint32_t value, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3);
uint32_t find_label(Label *labels, int label_count, const char *name);
int takeRegister(const char *key);
