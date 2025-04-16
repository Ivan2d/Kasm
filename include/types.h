#pragma once
#include <inttypes.h>

typedef enum {
  T_INS = 0x00,
  T_INT = 0x01,
  T_LAB = 0x02,
  T_REG = 0x03,
  T_DREG = 0x04,
  T_ID = 0x05,
  T_ADDRB = 0x06,
  T_ADDRW = 0x07,
  T_ADDRH = 0x08,
  T_BYTE = 0x09,
  T_STR = 0x0A,
  T_MCR = 0x0B,
  T_DREB = 0x0C,
  T_DREW = 0x0D,
  T_DREH = 0x0E,
  T_EOL = 0x0F,
  T_EOF = 0x10
} TokenType;

typedef enum {
  ASTINS = 0x00,
  ASTBYTES = 0x01,
  ASTRES = 0x02,
  ASTEOF = 0x04
} AstNodeType;

typedef enum {
  AX = 0x00,
  BX = 0x01,
  CX = 0x02,
  DX = 0x03,
  SI = 0x04,
  GI = 0x05,
  SP = 0x06,
  BP = 0x07
} Registers;

typedef struct {
  int type;
  char *value;
} Token;

typedef struct {
  char *name;
  uint32_t value;
} Label;

typedef struct {
  int astType;
  char *type;
  char *params[2];
  int pos;
  int pc;
} ASTInstruction;

typedef struct {
  ASTInstruction *ast;
  Label *labels;
  int ast_count;
  int label_count;
} ASTAndLabels;

typedef struct {
  uint8_t *bytes;
  int size;
  int new_pos;
} CompiledInstruction;
