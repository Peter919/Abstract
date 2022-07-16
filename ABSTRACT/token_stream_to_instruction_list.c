#include "list.h"
#include "token.h"
#include "stack.h"
#include "buffer.h"
#include "instruction.h"

static stack_element_t * new_stack_element_t(stack_element_t value)
{
        stack_element_t * element = malloc(sizeof(stack_element_t));
        *element = value;
        return element;
}

static void append_number(struct List * list, double number)
{
        // amount of elements to push, aka 1, is appended first
        list_append(list, (void *) new_stack_element_t(1));
        list_append(list, (void *) new_stack_element_t(number));
}

static void append_buffer(struct List * list, struct Buffer * buffer)
{
        list_append(list, (void *) new_stack_element_t(buffer->size));
        for (int i = 0; i < buffer->size; i++) {
                list_append(list, (void *) new_stack_element_t(buffer->contents[i]));
        }
}

struct List token_stream_to_instruction_list(struct List * token_stream);

static void append_code_block(struct List * list, struct List * code_block)
{
        struct List instr_list = token_stream_to_instruction_list(code_block);
        list_delete(code_block, free_token);

        list_append(list, (void *) new_stack_element_t(instr_list.length));
        list_append_list(list, &instr_list);
}

static struct List tok_to_instr_list(struct Token * token)
{
        struct List instr_list = (struct List) {0, 0, 0};

        stack_element_t * instr = malloc(sizeof(stack_element_t));
        *instr = token_to_instruction(token->type);

        list_append(&instr_list, (void *) instr);

        switch (token->type) {
        case TOK_IDENTIFIER:
        case TOK_NUMBER:
                append_number(&instr_list, *(double *) token->value);
                break;
        case TOK_STRING:
        case TOK_CHARACTERS:
                append_buffer(&instr_list, (struct Buffer *) token->value);
                break;
        case TOK_CODE_BLOCK:
                append_code_block(&instr_list, (struct List *) token->value);
                break;
        default:
                break;
        }

        return instr_list;
}

struct List token_stream_to_instruction_list(struct List * token_stream)
{
        struct List instr_list = (struct List) {0, 0, 0};
        struct List curr_tok_as_list;

        struct Node * node = token_stream->start;
        struct Token * token;

        while (node) {
                token = (struct Token *) node->value;

                curr_tok_as_list = tok_to_instr_list(token);
                list_append_list(&instr_list, &curr_tok_as_list);

                node = node->next;
        }

        return instr_list;
}
