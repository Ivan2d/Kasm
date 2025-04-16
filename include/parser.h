#include "types.h"
#include "utils.h"

ASTInstruction parse_inst(Token tokens[], char *b, int pos, int pc);
ASTAndLabels *parse(Token *tokens, const char *filename, int expm);
