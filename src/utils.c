#include "../include/utils.h"

const char *HUMAN_TOKS[] = {"inst",   "int",   "label", "reg",   "*reg",
                            "id",     "#addr", "@addr", "*addr", "byte",
                            "string", "mcr",   "eol",   "eof"};

const char *HUMAN_AST[] = {"INST", "BYTES", "RESERVE", "BRK-HANDLER", "EOF"};

void PrintTokens(Token toks[], int count) {
  for (int i = 0; i < count; i++) {
    if (toks[i].value != NULL) {
      printf("%d: %s %s\n", i, HUMAN_TOKS[toks[i].type], toks[i].value);
    } else {
      printf("%d: %s\n", i, HUMAN_TOKS[toks[i].type]);
    }
  }
}

void PrintAst(int toks[][4], int count) {
  for (int i = 0; i < count; i++) {
    printf("$%04X: %s %d %d\n", toks[i][3], HUMAN_AST[toks[i][0]], toks[i][1],
           toks[i][2]);
  }
}

void PrintLabs(Label labs[], int count) {
  printf("\nLabel symbols:\n");
  for (int i = 0; i < count; i++) {
    printf("%s: $%04X\n", labs[i].name, labs[i].value);
  }
}

void ExportLabs(const char *filename, Label labs[], int count) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file");
    return;
  }

  for (int i = 0; i < count; i++) {
    fprintf(file, "%s\t%04X\n", labs[i].name, labs[i].value);
  }

  fclose(file);
}

int ImportLabs(const char *filename, Label labs[], int max_count) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }

  int count = 0;
  char line[64];
  while (fgets(line, sizeof(line), file) && count < max_count) {
    char name[32];
    char value_str[16];
    if (sscanf(line, "%31s\t%15s", name, value_str) == 2) {
      labs[count].value = (int)strtol(value_str, NULL, 16);
      strncpy(labs[count].name, name, sizeof(labs[count].name) - 1);
      labs[count].name[sizeof(labs[count].name) - 1] = '\0';
      count++;
    }
  }

  fclose(file);
  return count;
}

void OffsetLabs(Label labs[], int count, int offset) {
  for (int i = 0; i < count; i++) {
    labs[i].value += offset;
  }
}

void Usage() {
  char *usage = "kasm -- Kakashka Assembler 1.0 (but who cares about the "
                "version anyways)\n"
                "Usage: kasm [OPTIONS] <file.asm> <file.bin>\n"
                "Options:\n"
                "  -h      Show help\n"
                "  -o [VAL]     Offset labels to VAL\n"
                "  -i <file>    Import labels from a file\n"
                "  -e           Export labels to the output file\n"
                "  -I           Start interactive mode\n";
  printf("%s", usage);
}

void freeTokens(Token *tokens) {
  for (size_t i = 0; i < token_count; i++) {
    free(tokens[i].value);
  }
  free(tokens);
}

void AsWord(uint16_t value, uint8_t *low, uint8_t *high) {
  *low = value & 0xFF;
  *high = (value >> 8) & 0xFF;
}

void As24(uint32_t value, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3) {
  *byte1 = value & 0xFF;
  *byte2 = (value >> 8) & 0xFF;
  *byte3 = (value >> 16) & 0xFF;
}

uint32_t find_label(Label *labels, int label_count, const char *name) {
  for (int i = 0; i < label_count; i++) {
    if (strcmp(labels[i].name, name) == 0) {
      return labels[i].value;
    }
  }
  fprintf(stderr, "Error: Label '%s' not found\n", name);
  exit(1);
}

int takeRegister(const char *key) {
  if (strcmp(key, "ax") == 0) {
    return AX;
  } else if (strcmp(key, "bx") == 0) {
    return BX;
  } else if (strcmp(key, "cx") == 0) {
    return CX;
  } else if (strcmp(key, "dx") == 0) {
    return DX;
  } else if (strcmp(key, "si") == 0) {
    return SI;
  } else if (strcmp(key, "gi") == 0) {
    return GI;
  } else if (strcmp(key, "sp") == 0) {
    return SP;
  } else if (strcmp(key, "bp") == 0) {
    return BP;
  } else {
    printf("%s %s", "kasm: error: unknown register -", key);
    exit(1);
  }
}
