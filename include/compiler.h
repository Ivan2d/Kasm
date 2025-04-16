#include "types.h"
#include "utils.h"

extern int code_count;

CompiledInstruction CompileInst(ASTInstruction *node, Label *labels,
                                int label_count, int pos);

uint8_t *CompileGC24(ASTInstruction *prog, Label *labels, int label_count,
                     int *exit_code);
