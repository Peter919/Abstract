#include "list.h"
#include "token.h"
#include "stack.h"
#include "token_stream_to_instruction_list.h"

void translate(struct List * token_stream, struct Stack * stack)
{
        logger(LOG_INFO, "Translating a token stream to a stack ...\n");

        struct List instr_list = token_stream_to_instruction_list(token_stream);

        struct Node * node = instr_list.start;
        while (node) {
                stack_push(stack, (stack_element_t *) node->value, 1);
                list_pop(&instr_list, 0, free);
                node = instr_list.start;
        }

        logger(LOG_SUCCESS, "Done translating a token stream to a stack.\n");

        logger(LOG_DEBUG, "Initial stack:\n");
        log_stack(LOG_DEBUG, stack, log_as_instruction_and_stack_element_t, ", ");
        newlines(LOG_DEBUG, 1);
        newlines(LOG_INFO, 1);
}
