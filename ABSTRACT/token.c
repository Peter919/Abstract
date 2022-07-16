#include "token.h"
#include <stdlib.h>
#include "log.h"
#include "list.h"
#include "buffer.h"

void free_token(void * token)
{
        struct Token * tokptr = (struct Token *) token;

        if (tokptr->value) {
                free(tokptr->value);
        }

        // fpath won't be freed as multiple tokens will use the same one

        free(tokptr);
}

static char * tok_as_str(enum e_Token token)
{
        switch (token) {
                case TOK_INVALID: return "invalid";
                case TOK_COMMENT: return "comment";
                case TOK_WHITESPACE: return "whitespace";
                case TOK_FILE: return "file";
                case TOK_IDENTIFIER: return "identifier";
                case TOK_EXIT: return "exit";
                case TOK_STACK_TOP_ADDRESS: return "stack top address";
                case TOK_SET_ADDRESS: return "set address";
                case TOK_GET_ADDRESS: return "get address";
                case TOK_STRING: return "string";
                case TOK_CHARACTERS: return "characters";
                case TOK_INSTRUCTIONS_START: return "instructions start";
                case TOK_INSTRUCTIONS_END: return "instructions end";
                case TOK_NUMBER: return "number";
                case TOK_INSTRUCTION_POINTER: return "instruction pointer";
                case TOK_ADD: return "add";
                case TOK_SUBTRACT: return "subtract";
                case TOK_MULTIPLY: return "multiply";
                case TOK_DIVIDE: return "divide";
                case TOK_MODULO: return "modulo";
                case TOK_POWER: return "power";
                case TOK_ROOT: return "root";
                case TOK_LOGARITHM: return "logarithm";
                case TOK_SINE: return "sine";
                case TOK_COSINE: return "cosine";
                case TOK_TANGENT: return "tangent";
                case TOK_ARCSINE: return "arcsine";
                case TOK_ARCCOSINE: return "arccosine";
                case TOK_ARCTANGENT: return "arctangent";
                case TOK_ROUND: return "round";
                case TOK_FLOOR: return "floor";
                case TOK_CEILING: return "ceiling";
                case TOK_INPUT_STRING: return "input string";
                case TOK_OUTPUT_STRING: return "output string";
                case TOK_INPUT_NUMBER: return "input number";
                case TOK_OUTPUT_NUMBER: return "output number";
                case TOK_FILE_READ: return "file read";
                case TOK_FILE_WRITE: return "file write";
                case TOK_FILE_READ_BINARY: return "file read binary";
                case TOK_FILE_WRITE_BINARY: return "file write binary";
                case TOK_CODE_BLOCK: return "code block";
                default: return "non-token argument";
        }
}

static void log_token_pre_or_post_parsing(enum e_LogLevel log_level, void * token, char post_parsing)
{
        struct Token * tokptr = (struct Token *) token;

        logger(log_level, tok_as_str(tokptr->type));

        switch (tokptr->type) {
        case TOK_FILE:
                logger(log_level, " (%s)", (char *) tokptr->value);
                break;
        case TOK_IDENTIFIER:
                if (post_parsing) {
                        logger(log_level, " (%lf)", *(double *) tokptr->value);
                } else {
                        logger(log_level, " (%s)", (char *) tokptr->value);
                }
                break;
        case TOK_STRING:
        case TOK_CHARACTERS:
                logger(log_level, " (");
                log_buffer_as_chars(log_level, tokptr->value);
                logger(log_level, ")");
                break;
        case TOK_NUMBER:
                logger(log_level, " (%lf)", *(double *) tokptr->value);
                break;
        case TOK_CODE_BLOCK:
                logger(log_level, " (");
                log_list(log_level, (struct List *) tokptr->value, post_parsing ? log_token_post_parsing : log_token, ", ");
                logger(log_level, ")");
                break;
        default:
                break;
        }
}

void log_token(enum e_LogLevel log_level, void * token)
{
        log_token_pre_or_post_parsing(log_level, token, 0);
}

void log_token_post_parsing(enum e_LogLevel log_level, void * token)
{
        log_token_pre_or_post_parsing(log_level, token, 1);
}
