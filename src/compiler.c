#include "../include/compiler.h"

int code_count = 0;

CompiledInstruction CompileInst(ASTInstruction *node, Label *labels,
                                int label_count, int pos) {
  CompiledInstruction result = {NULL, 0, pos};
  uint8_t buffer[64] = {0};
  int buffer_size = 0;

  printf("%s, %s\n", node->params[0], node->params[1]);
  if (strcmp(node->type, "MOVri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xC0 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVrl") == 0) {
    char *reg = (char *)node->params[0];
    char *label_name = (char *)(node->params)[1];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xC0 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVrb") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xD0 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVrw") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xD8 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVbr") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xE0 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVwr") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xE8 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0xCF;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "HLT") == 0) {
    buffer[buffer_size++] = 0x00;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MOVra") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xC0 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INT8") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    buffer[buffer_size++] = 0x41;
    buffer[buffer_size++] = takeRegister(reg);
    buffer[buffer_size++] = imm;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "POPr") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    buffer[buffer_size++] = 0xB6;
    buffer[buffer_size++] = takeRegister(reg);
    buffer[buffer_size++] = imm;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "NOP") == 0) {
    buffer[buffer_size++] = 0x05;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x48 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x47;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ANDrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x38;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ORArc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x39;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "XORrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x3A;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDrb") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x50 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDrw") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x58 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDbr") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x60 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "ADDwr") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x68 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JMPa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x86;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JMP2") == 0) {
    uint32_t address = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x86;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JEa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA0;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JNEa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA1;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JCa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA2;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JNCa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA3;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JSa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA4;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JNa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA5;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JIa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA6;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "JNIa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xA7;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LOOPa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xB8;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "SUBri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x10 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "SUBrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0xC8;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MULri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x08 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "MULrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0xC9;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DIVri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x80 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DIVrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0xCA;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INXr") == 0) {
    char *reg = (char *)node->params[0];
    buffer[buffer_size++] = 0x20 + takeRegister(reg);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DEXr") == 0) {
    char *reg = (char *)node->params[0];
    buffer[buffer_size++] = 0x28 + takeRegister(reg);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INXb") == 0) {
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x30;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INXw") == 0) {
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x40;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DEXb") == 0) {
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x32;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DEXw") == 0) {
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x42;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "CMPri") == 0) {
    char *reg = (char *)node->params[0];
    uint32_t imm = (uint32_t)strtol((char *)node->params[1], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x70 + takeRegister(reg);
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "CMPrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x37;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LODBc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x7F;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(reg);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LODWc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x8F;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LODHc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x9F;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STOBc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x7E;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STOWc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x8E;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STOHc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x9E;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "TRAP") == 0) {
    buffer[buffer_size++] = 0x01;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RET") == 0) {
    buffer[buffer_size++] = 0x79;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RE") == 0) {
    buffer[buffer_size++] = 0xA8;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RNE") == 0) {
    buffer[buffer_size++] = 0xA9;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RC") == 0) {
    buffer[buffer_size++] = 0xAA;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RNC") == 0) {
    buffer[buffer_size++] = 0xAB;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RS") == 0) {
    buffer[buffer_size++] = 0xAC;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RN") == 0) {
    buffer[buffer_size++] = 0xAD;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RI") == 0) {
    buffer[buffer_size++] = 0xAE;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "RNI") == 0) {
    buffer[buffer_size++] = 0xAF;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "PUSHi") == 0) {
    uint32_t imm = (uint32_t)strtol((char *)node->params[0], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(imm, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xB0;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "PUSHl") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0xB0;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "PUSHr") == 0) {
    char *reg = (char *)node->params[0];
    buffer[buffer_size++] = 0xB5;
    buffer[buffer_size++] = takeRegister(reg);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "CALLa") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x78;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "CALL2") == 0) {
    uint32_t address = (uint32_t)strtol((char *)node->params[0], NULL, 10);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x78;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LDDS") == 0) {
    buffer[buffer_size++] = 0xB9;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "LDDG") == 0) {
    buffer[buffer_size++] = 0xBA;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STDS") == 0) {
    buffer[buffer_size++] = 0xBB;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STDG") == 0) {
    buffer[buffer_size++] = 0xBC;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "STI8") == 0) {
    char *imm = (char *)node->params[0];
    buffer[buffer_size++] = 0x03;
    buffer[buffer_size++] = takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "IRET") == 0) {
    buffer[buffer_size++] = 0x04;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INXwl") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x40;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DEXwl") == 0) {
    const char *label_name = (char *)((void **)node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x42;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "INXbl") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x30;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "DEXbl") == 0) {
    const char *label_name = (char *)(node->params)[0];
    uint32_t address = find_label(labels, label_count, label_name);
    uint8_t byte1, byte2, byte3;
    As24(address, &byte1, &byte2, &byte3);
    buffer[buffer_size++] = 0x32;
    buffer[buffer_size++] = byte1;
    buffer[buffer_size++] = byte2;
    buffer[buffer_size++] = byte3;
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "POWrc") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0xBF;
    buffer[buffer_size++] = (takeRegister(reg) << 3) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "SALrg") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x7A;
    buffer[buffer_size++] = (takeRegister(reg) << 5) + takeRegister(imm);
    result.new_pos = pos + 1;
  } else if (strcmp(node->type, "SARrg") == 0) {
    char *reg = (char *)node->params[0];
    char *imm = (char *)node->params[1];
    buffer[buffer_size++] = 0x7B;
    buffer[buffer_size++] = (takeRegister(reg) << 5) + takeRegister(reg);
    result.new_pos = pos + 1;
  } else {
    fprintf(stderr, "kasm: unknown compilation node %s\n", node->type);
    exit(1);
  }

  result.bytes = malloc(buffer_size);
  memcpy(result.bytes, buffer, buffer_size);
  result.size = buffer_size;

  return result;
}

uint8_t *CompileGC24(ASTInstruction *prog, Label *labels, int label_count,
                     int *exit_code) {
  uint8_t *code = NULL;
  int code_size = 0;
  int pos = 0;

  while (prog[pos].astType != ASTEOF) {
    if (prog[pos].astType == ASTINS) {
      CompiledInstruction compiled =
          CompileInst(&prog[pos], labels, label_count, pos);
      if (compiled.new_pos == 0x1000000) {
        *exit_code = 1;
        free(code);
        return NULL;
      }

      uint8_t *new_code = realloc(code, code_size + compiled.size);
      if (!new_code) {
        *exit_code = 1;
        free(code);
        return NULL;
      }
      code = new_code;

      memcpy(code + code_size, compiled.bytes, compiled.size);
      code_size += compiled.size;
      code_count += compiled.size;

      free(compiled.bytes);
      pos = compiled.new_pos;
    } else if (prog[pos].astType == ASTBYTES) {
      if (prog[pos].params[1] != NULL) {
        const char *str = prog[pos].params[1];
        size_t len = strlen(str);

        uint8_t *new_code = realloc(code, code_size + len);
        if (!new_code) {
          *exit_code = 1;
          free(code);
          return NULL;
        }
        code = new_code;
        memcpy(code + code_size, str, len);
        code_size += len;
        code_count += len;
      } else if (prog[pos].params[0] != NULL) {
        char *endptr;
        unsigned long num = strtoul(prog[pos].params[0], &endptr, 0);
        if (*endptr != '\0' || num > 0xFFFFFFFF) {
          fprintf(stderr, "Error: invalid 32-bit value '%s'\n",
                  prog[pos].params[0]);
          *exit_code = 1;
          free(code);
          return NULL;
        }

        uint8_t bytes[4] = {(uint8_t)(num & 0xFF), (uint8_t)((num >> 8) & 0xFF),
                            (uint8_t)((num >> 16) & 0xFF),
                            (uint8_t)((num >> 24) & 0xFF)};

        // Добавляем 4 байта в код
        uint8_t *new_code = realloc(code, code_size + 4);
        if (!new_code) { /* обработка ошибки */
        }
        code = new_code;
        memcpy(code + code_size, bytes, 4);
        code_size += 4;
        code_count += 4;
      } else {
        fprintf(stderr, "Error: bytes directive has no data\n");
        *exit_code = 1;
        free(code);
        return NULL;
      }
      pos++;
    } else if (prog[pos].astType == ASTRES) {
      int reserve_size = atoi(prog[pos].params[0]);
      if (reserve_size <= 0) {
        fprintf(stderr, "Error: invalid reserve size\n");
        *exit_code = 1;
        free(code);
        return NULL;
      }

      uint8_t *new_code = realloc(code, code_size + reserve_size);
      if (!new_code) {
        *exit_code = 1;
        free(code);
        return NULL;
      }
      code = new_code;

      memset(code + code_size, 0, reserve_size);
      code_size += reserve_size;
      code_count += reserve_size;
      pos++;
    } else {
      fprintf(stderr, "kasm: unknown ast node\n");
      *exit_code = 1;
      free(code);
      return NULL;
    }
  }

  *exit_code = 0;
  return code;
}
