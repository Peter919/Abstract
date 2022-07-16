#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "token.h"
#include "settings.h"

struct IdString {
        unsigned int id;
        char * str;
        // used for warnings
        char used_more_than_once;
};

static void free_id_string(void * id_string)
{
        struct IdString * idstr_ptr = (struct IdString *) id_string;
        free(idstr_ptr->str);
        free(idstr_ptr);
}

static long id_of_str(struct List * id_string_list, char * str)
{
        struct Node * node = id_string_list->start;
        struct IdString * id_string;

        while (node) {
                id_string = (struct IdString *) node->value;

                if (strcmp(str, id_string->str) == 0) {
                        id_string->used_more_than_once = 1;
                        return id_string->id;
                }

                node = node->next;
        }

        return -1;
}

static char * copy_string(char * str)
{
        char * new_str = malloc(strlen(str) + 1);
        strcpy(new_str, str);
        return new_str;
}

static struct IdString * new_id_string(unsigned int id, char * str)
{
        struct IdString * id_string = malloc(sizeof(struct IdString));
        id_string->id = id;
        id_string->str = copy_string(str);
        id_string->used_more_than_once = 0;
        return id_string;
}

static void append_identifier_to_list(struct Token * identifier, struct List * id_string_list, unsigned int * id)
{
        if (identifier->type != TOK_IDENTIFIER) {
                return;
        }

        char * identifier_name = (char *) identifier->value;

        if (id_of_str(id_string_list, identifier_name) != -1) {
                return;
        }

        struct IdString * id_str = new_id_string(*id, identifier_name);

        list_append(id_string_list, (void *) id_str);

        ++*id;
}

static struct List get_id_strings(struct List * token_stream)
{
        struct List id_string_list = (struct List) {0, 0, 0};

        struct Node * node = token_stream->start;
        struct Token * token;
        unsigned int id = 0;

        while (node) {
                token = (struct Token *) node->value;

                append_identifier_to_list(token, &id_string_list, &id);

                node = node->next;
        }

        return id_string_list;
}

static void unused_identifier_warnings(struct List * id_str_list)
{
        struct Node * node = id_str_list->start;
        struct IdString * id_str;

        while (node) {
                id_str = (struct IdString *) node->value;

                if (!id_str->used_more_than_once) {
                        logger(LOG_WARNING, "Identifier %s only used once.\n", id_str->str);
                }

                node = node->next;
        }
}

static double * new_double(double value)
{
        double * new_val = malloc(sizeof(double));
        *new_val = value;
        return new_val;
}

unsigned int identifiers_to_ids(struct List * token_stream)
{
        struct List id_str_list = get_id_strings(token_stream);

        struct Node * node = token_stream->start;
        struct Token * token;
        unsigned int id;

        unused_identifier_warnings(&id_str_list);

        while (node) {
                token = (struct Token *) node->value;

                if (token->type == TOK_IDENTIFIER) {
                        // add one because ip has id 0 so all others are offset
                        id = id_of_str(&id_str_list, (char *) token->value) + 1;
                        free(token->value);
                        token->value = (void *) new_double((double) id);
                }

                node = node->next;
        }

        unsigned int amount_of_identifiers = id_str_list.length;
        list_delete(&id_str_list, free_id_string);

        return amount_of_identifiers;
}
