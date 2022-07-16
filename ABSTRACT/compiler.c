#include <stdlib.h>
#include <string.h>
#include "file_handling.h"
#include "list.h"
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "token.h"
#include "translation.h"

#define INDEX_OF_IP 0

static double * new_double(double value)
{
        double * new_val = malloc(sizeof(double));
        *new_val = value;
        return new_val;
}

static void replace_ip_with_numbers(struct List * token_stream)
{
        struct Node * node = token_stream->start;
        struct Token * token;
        while (node) {
                token = node->value;

                if (token->type == TOK_INSTRUCTION_POINTER) {
                        token->type = TOK_NUMBER;
                        token->value = (void *) new_double(INDEX_OF_IP);
                } else if (token->type == TOK_CODE_BLOCK) {
                        replace_ip_with_numbers((struct List *) token->value);
                }

                node = node->next;
        }
}

struct Stack compile(char * fpath)
{
        logger(LOG_INFO, "Compiling \"%s\" ...\n", fpath);

        char * file_contents = file_to_string(fpath);
        if (!file_contents) {
                return (struct Stack) {0, 0, 0};
        }

        struct List lexed_file = lex(fpath, file_contents);
        if (lexed_file.length == ~0) {
                return (struct Stack) {0, 0, 0};
        }
        free(file_contents);

        long amount_of_identifiers = parse(&lexed_file);
        if (amount_of_identifiers == -1) {
                return (struct Stack) {0, 0, 0};
        }

        replace_ip_with_numbers(&lexed_file);

        struct Stack stack = new_stack();
        // the stack will already contain one value, so ip is "allocated" in the beginning
        stack.top += amount_of_identifiers;
        memset(stack.elements, 0, sizeof(stack_element_t) * stack.top);
        // makes ip point to the first element after the variables, aka the first instruction
        stack.elements[0] = stack.top + 1;

        translate(&lexed_file, &stack);
        list_delete(&lexed_file, free_token);

        logger(LOG_SUCCESS, "Done compiling \"%s\".\n", fpath);
        return stack;
}
