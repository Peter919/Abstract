#include <stdlib.h>
#include "list.h"
#include "token.h"

static unsigned int index_of_closing_bracket(struct List * token_stream, unsigned int index_of_opening_bracket)
{
        struct Node * node = list_index_as_node(token_stream, index_of_opening_bracket);
        struct Token * token;
        unsigned int depth = 0;
        unsigned int index = index_of_opening_bracket;

        do {
                if (!node) {
                        logger(LOG_ERROR, "Expected \'}\' before EOF in \"%s\" ...\n", token->fpath);
                        return 0;
                }

                token = (struct Token *) node->value;
                if (token->type == TOK_INSTRUCTIONS_START) {
                        ++depth;
                } else if (token->type == TOK_INSTRUCTIONS_END) {
                        --depth;
                }

                node = node->next;
                ++index;
        } while (depth > 0);

        // reverses last increment of index from when the
        // depth was already equal to 0
        --index;

        return index;
}

char replace_code_blocks_with_tokens(struct List * token_stream);

static char replace_code_block(struct List * token_stream, unsigned int instr_start_index)
{
        struct Node * node = list_index_as_node(token_stream, instr_start_index);
        struct Token * token = (struct Token *) node->value;

        if (token->type != TOK_INSTRUCTIONS_START) {
                return 1;
        }

        // initializes basic values of code_block before token is freed
        struct Token * code_block = malloc(sizeof(struct Token));
        code_block->fpath = token->fpath;
        code_block->line = token->line;

        unsigned int instr_end_index = index_of_closing_bracket(token_stream, instr_start_index);
        if (instr_end_index == 0) {
                return 0;
        }

        list_pop(token_stream, instr_start_index, free_token);
        list_pop(token_stream, --instr_end_index, free_token);

        struct List * new_token_stream = malloc(sizeof(struct List));
        *new_token_stream = list_release_sublist(token_stream, instr_start_index, instr_end_index);
        if (!replace_code_blocks_with_tokens(new_token_stream)) {
                return 0;
        }

        code_block->type = TOK_CODE_BLOCK;
        code_block->value = (void *) new_token_stream;

        list_insert(token_stream, instr_start_index, (void *) code_block);

        return 1;
}

char replace_code_blocks_with_tokens(struct List * token_stream)
{
        for (int index = 0; index < token_stream->length; index++) {
                if (!replace_code_block(token_stream, index)) {
                        return 0;
                }
        }
        return 1;
}
