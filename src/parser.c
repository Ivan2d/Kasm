#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTInstruction parse_inst(Token tokens[], char *b, int pos, int pc) {
  ASTInstruction result;
  result.astType = ASTINS;
  if (strcmp(b, "mov") == 0) {
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
    if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_INT) {
      result.type = "MOVri";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_ID) {
      result.type = "MOVrl";
    } else if (tokens[pos + 1].type == T_REG &&
               tokens[pos + 2].type == T_ADDRB) {
      result.type = "MOVrb";
    } else if (tokens[pos + 1].type == T_REG &&
               tokens[pos + 2].type == T_ADDRW) {
      result.type = "MOVrw";
    } else if (tokens[pos + 1].type == T_ADDRB &&
               tokens[pos + 2].type == T_REG) {
      result.type = "MOVbr";
    } else if (tokens[pos + 1].type == T_ADDRW &&
               tokens[pos + 2].type == T_REG) {
      result.type = "MOVwr";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_REG) {
      result.type = "MOVrc";
      result.pc = pc + 2;
    }
  } else if (strcmp(b, "int") == 0 && tokens[pos + 1].type == T_INT) {
    result.type = "INT8";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 2;
  } else if (strcmp(b, "pop") == 0 && tokens[pos + 1].type == T_REG) {
    result.type = "POPr";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 2;
  } else if (strcmp(b, "add") == 0) {
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
    if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_INT) {
      result.type = "ADDri";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_REG) {
      result.type = "ADDrc";
      result.pc = pc + 2;
    } else if (tokens[pos + 1].type == T_REG &&
               tokens[pos + 2].type == T_ADDRB) {
      result.type = "ADDrb";
    } else if (tokens[pos + 1].type == T_REG &&
               tokens[pos + 2].type == T_ADDRW) {
      result.type = "ADDrw";
    } else if (tokens[pos + 1].type == T_ADDRB &&
               tokens[pos + 2].type == T_REG) {
      result.type = "ADDbr";
    } else if (tokens[pos + 1].type == T_ADDRW &&
               tokens[pos + 2].type == T_REG) {
      result.type = "ADDwr";
    }
  } else if (strcmp(b, "sub") == 0) {
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
    if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_INT) {
      result.type = "SUBri";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_REG) {
      result.type = "SUBrc";
      result.pc = pc + 2;
    }
  } else if (strcmp(b, "mul") == 0) {
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
    if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_INT) {
      result.type = "MULri";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_REG) {
      result.type = "MULrc";
      result.pc = pc + 2;
    }
  } else if (strcmp(b, "div") == 0) {
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
    if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_INT) {
      result.type = "DIVri";
    } else if (tokens[pos + 1].type == T_REG && tokens[pos + 2].type == T_REG) {
      result.type = "DIVrc";
      result.pc = pc + 2;
    }
  } else if (strcmp(b, "and") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "ANDrc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "ora") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "ORArc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "xor") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "XORrc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "jmp") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JMPa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jmp") == 0 && tokens[pos + 1].type == T_INT) {
    result.type = "JMP2";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "je") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JEa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jne") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JNEa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jc") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JCa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jnc") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JNCa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "js") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JSa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jg") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JSa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jn") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JNa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jl") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JNa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "ji") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JIa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "jni") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "JNIa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "loop") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "LOOPa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "inx") == 0 && tokens[pos + 1].type == T_REG) {
    result.type = "INXr";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 1;
  } else if (strcmp(b, "dex") == 0 && tokens[pos + 1].type == T_REG) {
    result.type = "DEXr";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 1;
  } else if (strcmp(b, "inx") == 0 && tokens[pos + 1].type == T_ADDRB) {
    result.type = "INXb";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "inx") == 0 && tokens[pos + 1].type == T_ADDRW) {
    result.type = "INXw";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "dex") == 0 && tokens[pos + 1].type == T_ADDRB) {
    result.type = "DEXb";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "dex") == 0 && tokens[pos + 1].type == T_ADDRW) {
    result.type = "DEXw";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "inx") == 0 && tokens[pos + 1].type == T_DREB) {
    result.type = "INXbl";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "dex") == 0 && tokens[pos + 1].type == T_DREB) {
    result.type = "DEXbl";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "inx") == 0 && tokens[pos + 1].type == T_DREW) {
    result.type = "INXwl";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "dex") == 0 && tokens[pos + 1].type == T_DREW) {
    result.type = "DEXwl";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "call") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "CALLa";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "call") == 0 && tokens[pos + 1].type == T_INT) {
    result.type = "CALL2";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "cmp") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_INT) {
    result.type = "CMPri";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 4;
  } else if (strcmp(b, "cmp") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "CMPrc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "lodb") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "LODBc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "lodw") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "LODWc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "lodh") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "LODWc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "stob") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "STOBc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "stow") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "STOWc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "stoh") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "STOHc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "pow") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_REG) {
    result.type = "POWrc";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "sal") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_INT) {
    result.type = "SALrg";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "sar") == 0 && tokens[pos + 1].type == T_REG &&
             tokens[pos + 2].type == T_INT) {
    result.type = "SARrg";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = tokens[pos + 2].value;
    result.pos = pos + 3;
    result.pc = pc + 2;
  } else if (strcmp(b, "push") == 0 && tokens[pos + 1].type == T_INT) {
    result.type = "PUSHi";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "push") == 0 && tokens[pos + 1].type == T_ID) {
    result.type = "PUSHl";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 4;
  } else if (strcmp(b, "push") == 0 && tokens[pos + 1].type == T_REG) {
    result.type = "PUSHr";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 2;
  } else if (strcmp(b, "sti") == 0 && tokens[pos + 1].type == T_INT) {
    result.type = "STI8";
    result.params[0] = tokens[pos + 1].value;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 2;
  } else if (strcmp(b, "hlt") == 0) {
    result.type = "HLT";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "trap") == 0) {
    result.type = "TRAP";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "ret") == 0) {
    result.type = "RET";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "re") == 0) {
    result.type = "RE";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 1;
  } else if (strcmp(b, "rne") == 0) {
    result.type = "RNE";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rc") == 0) {
    result.type = "RC";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rnc") == 0) {
    result.type = "RNC";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rs") == 0) {
    result.type = "RS";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rg") == 0) {
    result.type = "RS";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rn") == 0) {
    result.type = "RN";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rl") == 0) {
    result.type = "RN";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "ri") == 0) {
    result.type = "RI";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "rni") == 0) {
    result.type = "RNI";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "ldds") == 0) {
    result.type = "LDDS";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "lddg") == 0) {
    result.type = "LDDG";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "stds") == 0) {
    result.type = "STDS";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "stdg") == 0) {
    result.type = "STDG";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "nop") == 0) {
    result.type = "NOP";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 1;
    result.pc = pc + 1;
  } else if (strcmp(b, "iret") == 0) {
    result.type = "IRET";
    result.params[0] = NULL;
    result.params[1] = NULL;
    result.pos = pos + 2;
    result.pc = pc + 1;
  } else {
    printf("kasm: unknown instruction %d | %s", tokens[pos].type,
           tokens[pos].value);
    exit(EXIT_FAILURE);
  }
  return result;
}

ASTAndLabels *parse(Token *tokens, const char *filename, int expm) {
  ASTAndLabels *result = malloc(sizeof(ASTAndLabels));
  if (!result) {
    freeTokens(tokens);
    fprintf(stderr, "Memory allocation failed for ASTAndLabels\n");
    exit(1);
  }

  result->ast = NULL;
  result->ast_count = 0;
  result->labels = NULL;
  result->label_count = 0;

  int pos = 0;
  int pc = 0;
  int capacity = 1024;

  result->ast = malloc(capacity * sizeof(ASTInstruction));
  if (!result->ast) {
    fprintf(stderr, "Memory allocation failed for AST\n");
    freeTokens(tokens);
    free(result);
    exit(1);
  }

  while (tokens[pos].type != T_EOF) {
    if (result->ast_count >= capacity) {
      capacity *= 2;
      ASTInstruction *temp =
          realloc(result->ast, capacity * sizeof(ASTInstruction));
      if (!temp) {
        fprintf(stderr, "Memory reallocation failed for AST\n");
        freeTokens(tokens);
        free(result->labels);
        free(result->ast);
        free(result);
        exit(1);
      }
      result->ast = temp;
    }

    if (tokens[pos].type == T_INS) {
      ASTInstruction inst = parse_inst(tokens, tokens[pos].value, pos, pc);
      result->ast[result->ast_count++] = inst;
      printf("%s, %s\n", inst.params[0], inst.params[1]);
      pos = inst.pos;
      pc = inst.pc;
    } else if (tokens[pos].type == T_LAB) {
      Label *temp =
          realloc(result->labels, (result->label_count + 1) * sizeof(Label));
      if (!temp) {
        fprintf(stderr, "Memory allocation failed for labels\n");
        freeTokens(tokens);
        free(result->ast);
        free(result->labels);
        free(result);
        exit(1);
      }
      result->labels = temp;
      result->labels[result->label_count].name = tokens[pos].value;
      if (!result->labels[result->label_count].name) {
        fprintf(stderr, "Memory allocation failed for label name\n");
        freeTokens(tokens);
        free(result->ast);
        free(result->labels);
        free(result);
        exit(1);
      }
      result->labels[result->label_count].value = pc;
      result->label_count++;
      pos++;
    } else if (tokens[pos].type == T_BYTE) {
      result->ast[result->ast_count].astType = ASTBYTES;
      result->ast[result->ast_count].type = "__B_raw";
      result->ast[result->ast_count].params[0] = NULL;
      result->ast[result->ast_count].params[1] = NULL;
      result->ast[result->ast_count].pc = pc;
      result->ast_count++;
      pos++;

      while (tokens[pos].type != T_EOL) {
        if (tokens[pos].type == T_INT) {
          result->ast[result->ast_count - 1].params[0] = tokens[pos].value;
          pc++;
        } else if (tokens[pos].type == T_STR) {
          result->ast[result->ast_count - 1].params[1] = tokens[pos].value;
          pc += strlen(tokens[pos].value);
        } else {
          fprintf(stderr, "Error: invalid data in bytes directive\n");
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }
        pos++;
      }
      pos++;
    } else if (tokens[pos].type == T_MCR) {
      if (strcmp(tokens[pos].value, "reserve") == 0) {
        if (tokens[pos + 1].type != T_INT) {
          fprintf(stderr, "Error: expected number after reserve\n");
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }

        if (tokens[pos + 2].type != T_BYTE) {
          fprintf(stderr, "Error: expected 'bytes' after reserve count\n");
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }

        result->ast[result->ast_count].astType = ASTRES;
        result->ast[result->ast_count].type = "__B_reserve";
        result->ast[result->ast_count].params[0] = tokens[pos + 1].value;
        result->ast[result->ast_count].params[1] = NULL;
        result->ast[result->ast_count].pc = pc;
        result->ast_count++;
        pc += atoi(tokens[pos + 1].value);
        pos += 3;
      } else if (strcmp(tokens[pos].value, "extern") == 0) {
        if (tokens[pos + 1].type != T_STR) {
          fprintf(stderr, "Error: expected filename after extern\n");
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }
        FILE *file = fopen(tokens[pos + 1].value, "rb");
        if (!file) {
          fprintf(stderr, "Error: cannot open file '%s'\n",
                  tokens[pos + 1].value);
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *content = malloc(file_size + 1);
        if (!content) {
          fclose(file);
          fprintf(stderr, "Memory allocation failed for file content\n");
          freeTokens(tokens);
          free(result->ast);
          free(result->labels);
          free(result);
          exit(1);
        }

        fread(content, 1, file_size, file);
        content[file_size] = '\0';
        fclose(file);

        result->ast[result->ast_count].astType = ASTRES;
        result->ast[result->ast_count].type = "__B_extern";
        result->ast[result->ast_count].params[0] = content;
        result->ast[result->ast_count].params[1] = NULL;
        result->ast[result->ast_count].pc = pc;
        result->ast_count++;

        pc += file_size;
        pos += 2;
      }
    } else if (tokens[pos].type == T_EOL) {
      pos++;
    } else {
      fprintf(stderr, "Error: unknown token type %d at position %d\n",
              tokens[pos].type, pos);
      freeTokens(tokens);
      free(result->ast);
      free(result->labels);
      free(result);
      exit(1);
    }
  }

  if (result->ast_count >= capacity) {
    capacity += 1;
    ASTInstruction *temp =
        realloc(result->ast, capacity * sizeof(ASTInstruction));
    if (!temp) {
      fprintf(stderr, "Memory reallocation failed for AST\n");
      freeTokens(tokens);
      free(result->ast);
      free(result->labels);
      free(result);
      exit(1);
    }
    result->ast = temp;
  }

  result->ast[result->ast_count].astType = ASTEOF;
  result->ast[result->ast_count].type = NULL;
  result->ast[result->ast_count].params[0] = NULL;
  result->ast[result->ast_count].params[1] = NULL;
  result->ast[result->ast_count].pc = pc;
  result->ast_count++;

  if (expm) {
    printf("%s: exporting %d bytes\n", filename, pc);
  } else {
    printf("%s: program size: %d bytes\n", filename, pc);
  }

  return result;
}
