#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "token.h"
#include "file_handling.h"
#include "lexer.h"

static char * combine_strings(char * str1, char * str2)
{
        char * str = malloc(strlen(str1) + strlen(str2) + 1);
        strcpy(str, str1);
        strcat(str, str2);
        return str;
}

static char insert_file_contents(char * dirpath, struct List * token_stream, unsigned int index)
{
        struct Token * token = (struct Token *) list_index(token_stream, index);
        if (token->type != TOK_FILE) {
                return 1;
        }

        char * fpath = (char *) token->value;
        char is_fpath_absolute = is_absolute_path(fpath);
        if (!is_fpath_absolute) {
                fpath = combine_strings(dirpath, fpath);
        }

        char * fcontents = file_to_string(fpath);
        if (!fcontents) {
                logger(LOG_ERROR, "Couldn't open %s file path \"%s\".\n",
                       is_fpath_absolute ? "absolute" : "relative", fpath);
                return 0;
        }

        struct List lexed_subprogram = lex(fpath, fcontents);
        if (lexed_subprogram.length == ~0) {
                return 0;
        }

        if (!is_fpath_absolute) {
                free(fpath);
        }
        free(fcontents);

        list_pop(token_stream, index, free_token);
        list_insert_list(token_stream, index, &lexed_subprogram);
        return 2;
}

char replace_files_with_tokens(struct List * token_stream, char * dirpath)
{
        unsigned int index = 0;
        while (index < token_stream->length) {
                if (!insert_file_contents(dirpath, token_stream, index)) {
                        return 0;
                }

                ++index;
        }
        return 1;
}
