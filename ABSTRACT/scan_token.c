#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "get_token_type.h"
#include "token_scanning_functions.h"

struct Token * scan_token(char ** scanning_pos, char * fpath, unsigned int * line)
{
        struct Token * token = malloc(sizeof(struct Token));
        token->line = *line;
        token->fpath = fpath;
        token->type = get_token_type(*scanning_pos);
        token->value = 0;

        switch (token->type) {
        case TOK_INVALID:
                logger(LOG_ERROR, "Unexpected character \'%c\' in \"%s\", line %u.\n", **scanning_pos, fpath, *line);
                free_token((void *) token);
                return 0;
        case TOK_COMMENT:
                if (!scan_comment(scanning_pos, fpath)) {
                        return 0;
                }
                break;
        case TOK_WHITESPACE:
                if (**scanning_pos == '\n') {
                        ++*line;
                }
                ++*scanning_pos;
                break;
        case TOK_FILE:
                token->value = (void *) scan_file(scanning_pos, fpath);
                if (!token->value) {
                        return 0;
                }
                break;
        case TOK_IDENTIFIER:
                token->value = (void *) scan_identifier(scanning_pos);
                break;
        case TOK_EXIT:
                ++*scanning_pos;
                break;
        case TOK_STRING:
        case TOK_CHARACTERS:
                token->value = (void *) scan_string(scanning_pos, line, token->type == TOK_CHARACTERS);
                if (!token->value) {
                        logger(LOG_ERROR, "Error in string or characters in \"%s\", line %u.\n", fpath, *line);
                        free_token((void *) token);
                        return 0;
                }
                break;
        case TOK_INSTRUCTIONS_START:
        case TOK_INSTRUCTIONS_END:
                ++*scanning_pos;
                break;
        case TOK_NUMBER:
                token->value = (void *) scan_number(scanning_pos);
                break;
        case TOK_INSTRUCTION_POINTER:
                *scanning_pos += strlen("ip");
                break;
        case TOK_STACK_TOP_ADDRESS:
        case TOK_SET_ADDRESS:
        case TOK_GET_ADDRESS:
        case TOK_ADD:
        case TOK_SUBTRACT:
        case TOK_MULTIPLY:
        case TOK_DIVIDE:
        case TOK_MODULO:
        case TOK_POWER:
                ++*scanning_pos;
                break;
        case TOK_ROOT:
                *scanning_pos += strlen("root");
                break;
        case TOK_LOGARITHM:
                *scanning_pos += strlen("log");
                break;
        case TOK_SINE:
                *scanning_pos += strlen("sin");
                break;
        case TOK_COSINE:
                *scanning_pos += strlen("cos");
                break;
        case TOK_TANGENT:
                *scanning_pos += strlen("tan");
                break;
        case TOK_ARCSINE:
                *scanning_pos += strlen("asin");
                break;
        case TOK_ARCCOSINE:
                *scanning_pos += strlen("acos");
                break;
        case TOK_ARCTANGENT:
                *scanning_pos += strlen("atan");
                break;
        case TOK_ABSOLUTE_VALUE:
                *scanning_pos += strlen("abs");
                break;
        case TOK_ROUND:
                *scanning_pos += strlen("round");
                break;
        case TOK_FLOOR:
                *scanning_pos += strlen("floor");
                break;
        case TOK_CEILING:
                *scanning_pos += strlen("ceil");
                break;
        case TOK_INPUT_STRING:
        case TOK_OUTPUT_STRING:
        case TOK_INPUT_NUMBER:
        case TOK_OUTPUT_NUMBER:
                ++*scanning_pos;
                break;
        case TOK_FILE_READ:
                *scanning_pos += strlen("filer");
                break;
        case TOK_FILE_WRITE:
                *scanning_pos += strlen("filew");
                break;
        case TOK_FILE_READ_BINARY:
                *scanning_pos += strlen("filerb");
                break;
        case TOK_FILE_WRITE_BINARY:
                *scanning_pos += strlen("filewb");
                break;
        default:
                logger(LOG_ERROR, "Something went wrong when trying to get the type of a "
                       "token in \"%s\", line %u.\n", fpath, *line);
                return 0;
        }

        return token;
}
