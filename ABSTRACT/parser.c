#include "log.h"
#include "list.h"
#include "token.h"
#include "replace_code_blocks_with_tokens.h"
#include "identifiers_to_ids.h"

static void remove_token_type(struct List * token_stream, enum e_Token token_type)
{
        struct Node * node = token_stream->start;
        struct Token * token;
        unsigned int index = 0;

        while (node) {
                token = (struct Token *) node->value;
                // changes node before (maybe) popping so it doesn't point
                // to a freed memory location
                node = node->next;

                if (token->type == token_type) {
                        list_pop(token_stream, index, free_token);
                } else {
                        ++index;
                }
        }
}

long parse(struct List * token_stream)
{
        logger(LOG_INFO, "Parsing a stream of tokens ...\n");

        remove_token_type(token_stream, TOK_WHITESPACE);
        remove_token_type(token_stream, TOK_COMMENT);

        unsigned int amount_of_identifiers;
        amount_of_identifiers = identifiers_to_ids(token_stream);

        if (!replace_code_blocks_with_tokens(token_stream)) {
                return -1;
        }

        logger(LOG_SUCCESS, "Done parsing a stream of tokens.\n");

        logger(LOG_DEBUG, "Token stream after parsing:\n");
        log_list(LOG_DEBUG, token_stream, log_token_post_parsing, ", ");
        newlines(LOG_DEBUG, 2);

        return amount_of_identifiers;
}
