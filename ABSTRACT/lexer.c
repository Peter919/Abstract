#include "list.h"
#include "token.h"
#include "scan_token.h"
#include "get_token_type.h"
#include "replace_files_with_tokens.h"
#include "file_handling.h"

static void uneven_code_block_warnings(struct List * token_stream)
{
        struct Node * node = token_stream->start;
        struct Token * token;
        int depth = 0;
        while (node) {
                token = (struct Token *) node->value;

                if (token->type == TOK_INSTRUCTIONS_START) {
                        ++depth;
                } else if (token->type == TOK_INSTRUCTIONS_END) {
                        --depth;
                }

                if (depth < 0) {
                        logger(LOG_WARNING, "Unexpected \'}\' in \"%s\", line %u.\n", token->fpath, token->line);
                }

                node = node->next;
        }

        if (depth > 0) {
                logger(LOG_WARNING, "Expected \'}\' before EOF in \"%s\".\n", token->fpath);
        }
}

struct List lex(char * fpath, char * file_contents)
{
        logger(LOG_INFO, "Performing lexical analysis on \"%s\" ...\n", fpath);

        struct List lexed_contents = {0, 0, 0};
        char * scanning_pos = file_contents;
        unsigned int line = 1;

        while (*scanning_pos) {
                struct Token * token = scan_token(&scanning_pos, fpath, &line);

                if (!token) {
                        // ~0 is used as an error code
                        return (struct List) {0, 0, ~0};
                }

                list_append(&lexed_contents, (void *) token);
        }

        uneven_code_block_warnings(&lexed_contents);

        char * dirpath = get_dirpath(fpath);
        if (!replace_files_with_tokens(&lexed_contents, dirpath)) {
                return (struct List) {0, 0, ~0};
        }

        logger(LOG_DEBUG, "Lexed version of \"%s\":\n", fpath);
        log_list(LOG_DEBUG, &lexed_contents, log_token, ", ");
        newlines(LOG_DEBUG, 2);

        logger(LOG_SUCCESS, "Done performing lexical analysis on \"%s\" ...\n", fpath);
        return lexed_contents;
}
