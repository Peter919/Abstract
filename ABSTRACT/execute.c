#include "stack.h"
#include "instruction.h"
#include "instruction_functions.h"
#include "settings.h"
#include "instruction_functions.h"

#define INVALID_INSTRUCTION_ERROR(instr) \
do { \
        logger(LOG_ERROR, "Invalid instruction "); \
        logger(LOG_ERROR, g_stack_element_t_format, instr); \
        logger(LOG_ERROR, ".\n"); \
        return 0; \
} while(0);

#define INDEX_OF_IP 0

static char (*instr_to_func(stack_element_t instr))(struct Stack * stack)
{
        enum e_Instruction instr_enum = (enum e_Instruction) instr;

        switch (instr_enum) {
                case INSTR_PUSH: return _push;
                case INSTR_EXIT: return 0;
                case INSTR_STACK_TOP_ADDRESS: return _stack_top_address;
                case INSTR_SET_ADDRESS: return _set_address;
                case INSTR_GET_ADDRESS: return _get_address;
                case INSTR_ADD: return _add;
                case INSTR_SUBTRACT: return _subtract;
                case INSTR_MULTIPLY: return _multiply;
                case INSTR_DIVIDE: return _divide;
                case INSTR_MODULO: return _modulo;
                case INSTR_POWER: return _power;
                case INSTR_ROOT: return _root;
                case INSTR_LOGARITHM: return _logarithm;
                case INSTR_SINE: return _sine;
                case INSTR_COSINE: return _cosine;
                case INSTR_TANGENT: return _tangent;
                case INSTR_ARCSINE: return _arcsine;
                case INSTR_ARCCOSINE: return _arccosine;
                case INSTR_ARCTANGENT: return _arctangent;
                case INSTR_ABSOLUTE_VALUE: return _absolute_value;
                case INSTR_ROUND: return _round;
                case INSTR_FLOOR: return _floor;
                case INSTR_CEILING: return _ceiling;
                case INSTR_INPUT_STRING: return _input_string;
                case INSTR_OUTPUT_STRING: return _output_string;
                case INSTR_INPUT_NUMBER: return _input_number;
                case INSTR_OUTPUT_NUMBER: return _output_number;
                case INSTR_FILE_READ: return _file_read;
                case INSTR_FILE_WRITE: return _file_write;
                case INSTR_FILE_READ_BINARY: return _file_read_binary;
                case INSTR_FILE_WRITE_BINARY: return _file_write_binary;
                default: INVALID_INSTRUCTION_ERROR(instr);
        }
}

static char execute_instruction(struct Stack * stack)
{
        if (stack->elements[INDEX_OF_IP] > stack->top || stack->elements[INDEX_OF_IP] < 0) {
                logger(LOG_ERROR, "Instruction pointer out of stack bounds.\n");
                return 0;
        }
        if (stack->elements[INDEX_OF_IP] != (int) stack->elements[INDEX_OF_IP]) {
                logger(LOG_ERROR, "Non-integer instruction pointer.\n");
        }

        stack_element_t instr = stack->elements[(int) stack->elements[INDEX_OF_IP]];
        char (*func)(struct Stack * stack) = instr_to_func(instr);

        if (!func) {
                return 0;
        }
        if (!func(stack)) {
                return 0;
        }

        ++stack->elements[INDEX_OF_IP];

        return 1;
}

void execute(struct Stack * stack)
{
        logger(LOG_INFO, "Executing program ...\n");

        while (execute_instruction(stack)) {}

        newlines(LOG_SUCCESS, 1);
        logger(LOG_SUCCESS, "Done executing program.\n");

        logger(LOG_DEBUG, "Final stack:\n");
        log_stack(LOG_DEBUG, stack, log_as_instruction_and_stack_element_t, ", ");

        newlines(LOG_SUCCESS, 1);
        newlines(LOG_NONE, 1);
}
