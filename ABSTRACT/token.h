#pragma once
#include "log.h"

enum e_Token {
        TOK_INVALID,

        TOK_COMMENT,
        TOK_WHITESPACE,
        TOK_FILE,
        TOK_IDENTIFIER,
        TOK_EXIT,

        TOK_STRING, TOK_CHARACTERS, TOK_INSTRUCTIONS_START, TOK_INSTRUCTIONS_END,
        TOK_NUMBER, TOK_INSTRUCTION_POINTER,

        TOK_STACK_TOP_ADDRESS, TOK_SET_ADDRESS, TOK_GET_ADDRESS,

        TOK_ADD, TOK_SUBTRACT, TOK_MULTIPLY, TOK_DIVIDE, TOK_MODULO, TOK_POWER,
        TOK_ROOT, TOK_LOGARITHM, TOK_SINE, TOK_COSINE, TOK_TANGENT, TOK_ARCSINE,
        TOK_ARCCOSINE, TOK_ARCTANGENT, TOK_ABSOLUTE_VALUE, TOK_ROUND, TOK_FLOOR,
        TOK_CEILING,

        TOK_INPUT_STRING, TOK_OUTPUT_STRING, TOK_INPUT_NUMBER, TOK_OUTPUT_NUMBER,

        TOK_FILE_READ, TOK_FILE_WRITE, TOK_FILE_READ_BINARY,
        TOK_FILE_WRITE_BINARY,

        TOK_AMOUNT_OF_STANDARD_TOKENS,

        // added during the parsing
        TOK_CODE_BLOCK
};

struct Token {
        enum e_Token type;
        void * value;
        char * fpath;
        unsigned int line;
};

void free_token(void * token);

void log_token(enum e_LogLevel log_level, void * token);

void log_token_post_parsing(enum e_LogLevel log_level, void * token);
