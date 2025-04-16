#include "../include/compiler.h"
#include "../include/lexer.h"
#include "../include/parser.h"

void IKasm() {
  printf("interactive kasm\n");

  while (1) {
    char input[256];
    printf(">>> \033[32m");
    if (!fgets(input, sizeof(input), stdin)) {
      printf("\033[0m");
      break;
    }
    printf("\033[0m");

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    Token *tokens = Lex(input, "input");
    if (!tokens) {
      fprintf(stderr, "Lexical error\n");
      continue;
    }

    ASTAndLabels *ast_and_labels = parse(tokens, "input", 0);
    if (!ast_and_labels) {
      fprintf(stderr, "Parse error\n");
      freeTokens(tokens);
      continue;
    }

    int exit_code = 0;
    uint8_t *code = CompileGC24(ast_and_labels->ast, ast_and_labels->labels,
                                ast_and_labels->label_count, &exit_code);
    if (exit_code != 0 || !code) {
      fprintf(stderr, "Compilation error\n");
      freeTokens(tokens);
      free(ast_and_labels->ast);
      free(ast_and_labels->labels);
      free(ast_and_labels);
      continue;
    }

    int code_size = 0;
    while (code[code_size] != code_count) {
      printf("%02X ", code[code_size]);
      code_size++;
    }
    printf("\033[0m\n");

    freeTokens(tokens);
    free(ast_and_labels->ast);
    free(ast_and_labels->labels);
    free(ast_and_labels);
    free(code);
  }
}

int main(int argc, char *argv[]) {
  int exportmode = 0;
  char *progname = NULL;
  char *outname = NULL;
  char **imp_files = NULL;
  int imp_count = 0;
  uint32_t offset = 0x030000;

  if (argc == 1) {
    printf("No arguments given\n");
    Usage();
    return 1;
  }

  int argp = 1;
  while (argp < argc) {
    if (strcmp(argv[argp], "-e") == 0) {
      exportmode = 1;
      argp++;
    } else if (strcmp(argv[argp], "-i") == 0) {
      if (argp + 1 >= argc) {
        fprintf(stderr, "Error: Missing argument for -i\n");
        return 1;
      }
      imp_files = realloc(imp_files, (imp_count + 1) * sizeof(char *));
      imp_files[imp_count++] = argv[argp + 1];
      argp += 2;
    } else if (strcmp(argv[argp], "-o") == 0) {
      if (argp + 1 >= argc) {
        fprintf(stderr, "Error: Missing argument for -o\n");
        return 1;
      }
      offset = strtol(argv[argp + 1], NULL, 16);
      argp += 2;
    } else if (strcmp(argv[argp], "-I") == 0) {
      IKasm();
      return 0;
    } else if (strcmp(argv[argp], "-h") == 0) {
      Usage();
      return 0;
    } else {
      progname = argv[argp];
      outname = argv[argp + 1];
      break;
    }
  }

  if (!progname || !outname) {
    fprintf(stderr, "Error: Missing input or output file\n");
    return 1;
  }

  FILE *fl = fopen(progname, "r");
  if (!fl) {
    perror("Error opening file");
    return 1;
  }

  fseek(fl, 0, SEEK_END);
  long flen = ftell(fl);
  fseek(fl, 0, SEEK_SET);

  char *src = malloc(flen + 1);
  if (!src) {
    fclose(fl);
    fprintf(stderr, "Memory allocation failed\n");
    return 1;
  }

  fread(src, 1, flen, fl);
  src[flen] = '\0';
  fclose(fl);

  Token *tokens = Lex(src, progname);
  if (!tokens) {
    fprintf(stderr, "Lexical error\n");
    free(src);
    return 1;
  }

  ASTAndLabels *ast_and_labels = parse(tokens, progname, exportmode);
  if (!ast_and_labels) {
    fprintf(stderr, "Parse error\n");
    freeTokens(tokens);
    free(src);
    return 1;
  }

  OffsetLabs(ast_and_labels->labels, ast_and_labels->label_count, offset);

  if (exportmode) {
    ExportLabs(outname, ast_and_labels->labels, ast_and_labels->label_count);
    freeTokens(tokens);
    free(ast_and_labels->ast);
    free(ast_and_labels->labels);
    free(ast_and_labels);
    free(src);
    return 0;
  }

  for (int i = 0; i < imp_count; i++) {
    ImportLabs(imp_files[i], ast_and_labels->labels,
               ast_and_labels->label_count);
  }

  int exit_code = 0;
  uint8_t *code = CompileGC24(ast_and_labels->ast, ast_and_labels->labels,
                              ast_and_labels->label_count, &exit_code);
  if (exit_code != 0 || !code) {
    fprintf(stderr, "Compilation error\n");
    freeTokens(tokens);
    free(ast_and_labels->ast);
    free(ast_and_labels->labels);
    free(ast_and_labels);
    free(src);
    return 1;
  }

  FILE *outfl = fopen(outname, "wb");
  if (!outfl) {
    perror("Error opening output file");
    freeTokens(tokens);
    free(ast_and_labels->ast);
    free(ast_and_labels->labels);
    free(ast_and_labels);
    free(src);
    free(code);
    return 1;
  }

  fwrite(code, 1, code_count, outfl);
  fclose(outfl);

  freeTokens(tokens);
  free(ast_and_labels->ast);
  free(ast_and_labels->labels);
  free(ast_and_labels);
  free(src);
  free(code);
  free(imp_files);

  return 0;
}
