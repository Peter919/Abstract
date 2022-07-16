#include "instruction.h"
#include "log.h"
#include "token.h"
#include "stack.h"

static const char * instr_to_str(enum e_Instruction instr)
{
        switch (instr) {
                case INSTR_INVALID: return "invalid";
                case INSTR_PUSH: return "push";
                case INSTR_EXIT: return "exit";
                case INSTR_STACK_TOP_ADDRESS: return "stack top address";
                case INSTR_SET_ADDRESS: return "set address";
                case INSTR_GET_ADDRESS: return "get address";
                case INSTR_ADD: return "add";
                case INSTR_SUBTRACT: return "subtract";
                case INSTR_MULTIPLY: return "multiply";
                case INSTR_DIVIDE: return "divide";
                case INSTR_MODULO: return "modulo";
                case INSTR_POWER: return "power";
                case INSTR_ROOT: return "root";
                case INSTR_LOGARITHM: return "logarithm";
                case INSTR_SINE: return "sine";
                case INSTR_COSINE: return "cosine";
                case INSTR_TANGENT: return "tangent";
                case INSTR_ARCSINE: return "arcsine";
                case INSTR_ARCCOSINE: return "arccosine";
                case INSTR_ARCTANGENT: return "arctangent";
                case INSTR_ABSOLUTE_VALUE: return "absolute value";
                case INSTR_ROUND: return "round";
                case INSTR_FLOOR: return "floor";
                case INSTR_CEILING: return "ceiling";
                case INSTR_INPUT_STRING: return "input string";
                case INSTR_OUTPUT_STRING: return "output string";
                case INSTR_INPUT_NUMBER: return "input number";
                case INSTR_OUTPUT_NUMBER: return "output number";
                case INSTR_FILE_READ: return "file read";
                case INSTR_FILE_WRITE: return "file write";
                case INSTR_FILE_READ_BINARY: return "file read binary";
                case INSTR_FILE_WRITE_BINARY: return "file write binary";
                default: return "non-instruction argument";
        }
}

void log_instruction(enum e_LogLevel log_level, void * instruction)
{
        stack_element_t * stack_elem_ptr = (stack_element_t *) instruction;
        enum e_Instruction instr_enum = (enum e_Instruction) *stack_elem_ptr;
        const char * log_string = instr_to_str(instr_enum);

        logger(log_level, "%s", log_string);
}

enum e_Instruction token_to_instruction(enum e_Token token)
{
        switch (token) {
                case TOK_IDENTIFIER:
                case TOK_STRING:
                case TOK_CHARACTERS:
                case TOK_NUMBER:
                case TOK_CODE_BLOCK: return INSTR_PUSH;
                case TOK_EXIT: return INSTR_EXIT;
                case TOK_STACK_TOP_ADDRESS: return INSTR_STACK_TOP_ADDRESS;
                case TOK_SET_ADDRESS: return INSTR_SET_ADDRESS;
                case TOK_GET_ADDRESS: return INSTR_GET_ADDRESS;
                case TOK_ADD: return INSTR_ADD;
                case TOK_SUBTRACT: return INSTR_SUBTRACT;
                case TOK_MULTIPLY: return INSTR_MULTIPLY;
                case TOK_DIVIDE: return INSTR_DIVIDE;
                case TOK_MODULO: return INSTR_MODULO;
                case TOK_POWER: return INSTR_POWER;
                case TOK_ROOT: return INSTR_ROOT;
                case TOK_LOGARITHM: return INSTR_LOGARITHM;
                case TOK_SINE: return INSTR_SINE;
                case TOK_COSINE: return INSTR_COSINE;
                case TOK_TANGENT: return INSTR_TANGENT;
                case TOK_ARCSINE: return INSTR_ARCSINE;
                case TOK_ARCCOSINE: return INSTR_ARCCOSINE;
                case TOK_ARCTANGENT: return INSTR_ARCTANGENT;
                case TOK_ABSOLUTE_VALUE: return INSTR_ABSOLUTE_VALUE;
                case TOK_ROUND: return INSTR_ROUND;
                case TOK_FLOOR: return INSTR_FLOOR;
                case TOK_CEILING: return INSTR_CEILING;
                case TOK_INPUT_STRING: return INSTR_INPUT_STRING;
                case TOK_OUTPUT_STRING: return INSTR_OUTPUT_STRING;
                case TOK_INPUT_NUMBER: return INSTR_INPUT_NUMBER;
                case TOK_OUTPUT_NUMBER: return INSTR_OUTPUT_NUMBER;
                case TOK_FILE_READ: return INSTR_FILE_READ;
                case TOK_FILE_WRITE: return INSTR_FILE_WRITE;
                case TOK_FILE_READ_BINARY: return INSTR_FILE_READ_BINARY;
                case TOK_FILE_WRITE_BINARY: return INSTR_FILE_WRITE_BINARY;
                default: return INSTR_INVALID;
        }
}
