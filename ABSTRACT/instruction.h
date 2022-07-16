#pragma once
#include "log.h"
#include "token.h"

enum e_Instruction {
        // not really used but needed to return something in token_to_instruction
        INSTR_INVALID,

        INSTR_PUSH,

        INSTR_EXIT,

        INSTR_STACK_TOP_ADDRESS, INSTR_SET_ADDRESS, INSTR_GET_ADDRESS,

        INSTR_ADD, INSTR_SUBTRACT, INSTR_MULTIPLY, INSTR_DIVIDE, INSTR_MODULO,
        INSTR_POWER, INSTR_ROOT, INSTR_LOGARITHM, INSTR_SINE, INSTR_COSINE,
        INSTR_TANGENT, INSTR_ARCSINE, INSTR_ARCCOSINE, INSTR_ARCTANGENT,
        INSTR_ABSOLUTE_VALUE, INSTR_ROUND, INSTR_FLOOR, INSTR_CEILING,

        INSTR_INPUT_STRING, INSTR_OUTPUT_STRING, INSTR_INPUT_NUMBER,
        INSTR_OUTPUT_NUMBER,

        INSTR_FILE_READ, INSTR_FILE_WRITE, INSTR_FILE_READ_BINARY,
        INSTR_FILE_WRITE_BINARY
};

void log_instruction(enum e_LogLevel log_level, void * instruction);

enum e_Instruction token_to_instruction(enum e_Token token);
