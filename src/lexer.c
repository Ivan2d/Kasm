#include "../include/lexer.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHI(c) (isspace(c) && (c) != '\n')
#define LET(c) (isalpha(c) || (c) == '_')
#define DIG(c) (isdigit(c))
#define LETEXT(c) (isalnum(c) || (c) == '_' || (c) == '-' || (c) == '.')
#define DIGEXT(c) (isxdigit(c))

int token_count = 0;

char *KEY2[] = {"mov",  "nop",  "int",  "hlt",  "add",  "jmp",  "inx",  "dex",
                "je",   "jne",  "jc",   "jnc",  "js",   "jn",   "ji",   "jni",
                "cmp",  "jg",   "jl",   "lodb", "lodw", "lodh", "trap", "call",
                "ret",  "re",   "rne",  "rc",   "rnc",  "rs",   "rn",   "ri",
                "rni",  "push", "stob", "stow", "stoh", "mul",  "div",  "sub",
                "loop", "pop",  "ldds", "lddg", "stds", "stdg", "sti",  "iret",
                "pow",  "and",  "ora",  "xor",  "sal",  "sar"};

char *KEYR[] = {"ax", "bx", "cx", "dx", "si", "gi", "sp", "bp"};

Token *Lex(const char *prog, const char *filename) {
  size_t proglen = strlen(prog);
  Token *tokens = malloc((proglen + 2) * sizeof(Token));
  if (!tokens) {
    perror("Memory allocation error");
    exit(1);
  }

  int pos = 0;
  int linenum = 1;
  int token_index = 0;
  char buf[256] = {0};
  char labelscope[256] = {0};

  while (pos < proglen) {
    char c = prog[pos];

    if (c == '\0') {
      tokens[token_index++] = (Token){T_EOL, strdup("")};
      tokens[token_index++] = (Token){T_EOF, strdup("")};
      break;
    }

    else if (c == ';') {
      pos++;
      while (prog[pos] != '\n' && pos < proglen)
        pos++;
    } else if (c == '/' && prog[pos + 1] == '/') {
      pos += 2;
      while (prog[pos] != '\n' && pos < proglen)
        pos++;
    } else if (c == '/' && prog[pos + 1] == '*') {
      pos += 2;
      while (pos < proglen - 1 && !(prog[pos] == '*' && prog[pos + 1] == '/')) {
        if (prog[pos] == '\n')
          linenum++;
        pos++;
      }
      if (pos >= proglen - 1) {
        fprintf(stderr, "%s:%d: Unterminated comment\n", filename, linenum);
        exit(1);
      }
      pos += 2;
    }

    else if (WHI(c)) {
      pos++;
    } else if (c == '\n') {
      if (token_index > 0 && tokens[token_index - 1].type != T_EOL) {
        tokens[token_index++] = (Token){T_EOL, strdup("")};
        token_count++;
      }
      pos++;
      linenum++;
    }

    else if (c == '.') {
      pos++;
      int buf_len = 0;
      while (LETEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';

      if (prog[pos] == ':') {
        char full_label[512];
        snprintf(full_label, sizeof(full_label), "%s%s%s",
                 labelscope[0] ? labelscope : "", labelscope[0] ? "." : "",
                 buf);
        tokens[token_index++] = (Token){T_LAB, strdup(full_label)};
        token_count++;
        pos++;
      } else {
        char full_id[512];
        snprintf(full_id, sizeof(full_id), "%s%s%s",
                 labelscope[0] ? labelscope : "", labelscope[0] ? "." : "",
                 buf);
        tokens[token_index++] = (Token){T_ID, strdup(full_id)};
      }
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '%') {
      pos++;
      int buf_len = 0;
      while (LET(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';

      char found = 0;
      for (size_t i = 0; i < sizeof(KEYR) / sizeof(KEYR[0]); i++) {
        if (strcmp(buf, KEYR[i]) == 0) {
          tokens[token_index++] = (Token){T_REG, strdup(KEYR[i])};
          token_count++;
          found++;
          break;
        }
      }

      if (!found) {
        fprintf(stderr, "%s:%d: Unknown register `%s`\n", filename, linenum,
                buf);
        exit(1);
      }
      memset(buf, 0, sizeof(buf));
    }

    else if (DIG(c)) {
      int buf_len = 0;
      while (DIG(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';
      tokens[token_index++] = (Token){T_INT, strdup(buf)};
      token_count++;
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '"') {
      pos++;
      int buf_len = 0;
      while (prog[pos] != '"' && pos < proglen) {
        if (prog[pos] == '\\' && prog[pos + 1] == '"') {
          buf[buf_len++] = '"';
          pos += 2;
        } else if (prog[pos] == '\n') {
          fprintf(stderr, "%s:%d: Unterminated string literal\n", filename,
                  linenum);
          exit(1);
        } else {
          buf[buf_len++] = prog[pos++];
        }
      }
      if (prog[pos] != '"') {
        fprintf(stderr, "%s:%d: Unterminated string literal\n", filename,
                linenum);
        exit(1);
      }
      buf[buf_len] = '\0';
      pos++;
      tokens[token_index++] = (Token){T_STR, strdup(buf)};
      token_count++;
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '\'') {
      pos++;
      char char_val = 0;

      if (prog[pos] == '\\' && prog[pos + 1] == '\'') {
        char_val = '\'';
        pos += 2;
      } else if (prog[pos] == '\\' && prog[pos + 1] == '"') {
        char_val = '"';
        pos += 2;
      } else if (prog[pos] == '\\' && prog[pos + 1] == 'n') {
        char_val = '\n';
        pos += 2;
      } else {
        char_val = prog[pos++];
      }

      if (prog[pos] != '\'') {
        fprintf(stderr, "%s:%d: Unterminated character literal\n", filename,
                linenum);
        exit(1);
      }
      pos++;
      tokens[token_index++] = (Token){T_INT, strdup(&char_val)};
      token_count++;
    }

    else if (c == '^' || c == '$') {
      pos++;
      int buf_len = 0;
      while (DIGEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';
      tokens[token_index++] = (Token){T_INT, strdup(buf)};
      token_count++;
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '#') {
      pos++;
      int buf_len = 0;
      if (DIGEXT(prog[pos])) {
        while (DIGEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
          buf[buf_len++] = prog[pos++];
        }
        buf[buf_len] = '\0';
        tokens[token_index++] = (Token){T_ADDRB, strdup(buf)};
        token_count++;
      } else {
        while (LET(prog[pos]) && buf_len < sizeof(buf) - 1) {
          buf[buf_len++] = prog[pos++];
        }
        buf[buf_len] = '\0';
        tokens[token_index++] = (Token){T_DREB, strdup(buf)};
        token_count++;
      }
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '@') {
      pos++;
      int buf_len = 0;
      if (DIGEXT(prog[pos])) {
        while (DIGEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
          buf[buf_len++] = prog[pos++];
        }
        buf[buf_len] = '\0';
        tokens[token_index++] = (Token){T_ADDRW, strdup(buf)};
        token_count++;
      } else {
        while (LET(prog[pos]) && buf_len < sizeof(buf) - 1) {
          buf[buf_len++] = prog[pos++];
        }
        buf[buf_len] = '\0';
        tokens[token_index++] = (Token){T_DREW, strdup(buf)};
        token_count++;
      }
      memset(buf, 0, sizeof(buf));
    }

    else if (c == '*') {
      pos++;
      int regop = 0;
      if (prog[pos] == '%') {
        regop++;
        pos++;
      }

      int buf_len = 0;
      while (LETEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';

      if (regop) {
        int found = 0;
        for (size_t i = 0; i < sizeof(KEYR) / sizeof(KEYR[0]); i++) {
          if (strcmp(buf, KEYR[i]) == 0) {
            tokens[token_index++] = (Token){T_DREG, strdup(buf)};
            token_count++;
            found++;
            break;
          }
        }
        if (!found) {
          fprintf(stderr, "%s:%d: Unknown register `%s`\n", filename, linenum,
                  buf);
          exit(1);
        }
      } else {
        tokens[token_index++] = (Token){T_ID, strdup(buf)};
        token_count++;
      }
      memset(buf, 0, sizeof(buf));
    }

    else if (LET(c) || c == '_') {
      int buf_len = 0;
      while (LETEXT(prog[pos]) && buf_len < sizeof(buf) - 1) {
        buf[buf_len++] = prog[pos++];
      }
      buf[buf_len] = '\0';

      if (prog[pos] == ':') {
        tokens[token_index++] = (Token){T_LAB, strdup(buf)};
        token_count++;
        strncpy(labelscope, buf, sizeof(labelscope) - 1);
        labelscope[sizeof(labelscope) - 1] = '\0';
        pos++;
      } else {
        int is_keyword = 0;
        for (size_t i = 0; i < sizeof(KEY2) / sizeof(KEY2[0]); i++) {
          if (strcmp(buf, KEY2[i]) == 0) {
            tokens[token_index++] = (Token){T_INS, strdup(buf)};
            token_count++;
            is_keyword++;
            break;
          }
        }

        if (!is_keyword) {
          if (strcmp(buf, "bytes") == 0) {
            tokens[token_index++] = (Token){T_BYTE, strdup("0")};
            token_count++;
          } else if (strcmp(buf, "reserve") == 0) {
            tokens[token_index++] = (Token){T_MCR, strdup("reserve")};
            token_count++;
          } else if (strcmp(buf, "extern") == 0) {
            tokens[token_index++] = (Token){T_MCR, strdup("extern")};
          } else {
            tokens[token_index++] = (Token){T_ID, strdup(buf)};
            token_count++;
          }
        }
      }
      memset(buf, 0, sizeof(buf));
    } else {
      fprintf(stderr, "%s:%d: Unknown character `%c` (0x%02X)\n", filename,
              linenum, c, c);
      exit(1);
    }
  }
  if (token_index == 0 || tokens[token_index - 1].type != T_EOF) {
    tokens[token_index++] = (Token){T_EOF, strdup("")};
    token_count++;
  }
  return tokens;
}
