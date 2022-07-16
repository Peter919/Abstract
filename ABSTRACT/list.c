// everything needed for lists of void pointers (aka anything)

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static void free_node(struct Node * node, void (*free_value)(void *))
{
        free_value(node->value);
        free(node);
}

struct Node * list_index_as_node(struct List * list, unsigned int index)
{
        if (index < 0 || index >= list->length) {
                return 0;
        }

        struct Node * index_node = list->start;

        for (int i = 0; i < index; i++) {
                index_node = index_node->next;
        }

        return index_node;
}

void * list_index(struct List * list, unsigned int index)
{
        return list_index_as_node(list, index)->value;
}

void list_append(struct List * list, void * value)
{
        struct Node * node = malloc(sizeof(struct Node));
        node->value = value;
        node->next = 0;

        if (list->length == 0) {
                list->start = node;
        }
        else {
                list->end->next = node;
        }

        list->end = node;
        ++list->length;
}

void list_append_list(struct List * list1, struct List * list2)
{
        if (list1->length == 0) {
                *list1 = *list2;
                return;
        }
        if (list2->length == 0) {
                return;
        }

        list1->end->next = list2->start;
        list1->end = list2->end;
        list1->length += list2->length;
}

char list_insert(struct List * list, unsigned int index, void * value)
{
        if (index > list->length) {
                return 0;
        }
        if (index == list->length) {
                list_append(list, value);
                return 1;
        }

        struct Node * node = malloc(sizeof(struct Node));
        node->value = value;
        node->next = list_index_as_node(list, index);

        if (index == 0) {
                list->start = node;
        } else {
                list_index_as_node(list, index - 1)->next = node;
        }

        ++list->length;
        return 1;
}

char list_insert_list(struct List * list1, unsigned int index, struct List * list2)
{
        if (index > list1->length) {
                return 0;
        }
        if (index == list1->length) {
                list_append_list(list1, list2);
                return 1;
        }

        list2->end->next = list_index_as_node(list1, index);
        if (index == 0) {
                list1->start = list2->start;
        } else {
                list_index_as_node(list1, index - 1)->next = list2->start;
        }
        list1->length += list2->length;
        return 1;
}

char list_insert_after_node(struct Node * node, void * value)
{
        struct Node * new_node = malloc(sizeof(struct Node));
        new_node->value = value;
        new_node->next = node->next;

        node->next = new_node;
        return 1;
}

char list_pop(struct List * list, unsigned int index, void (* free_value)(void *))
{
        if (index >= list->length) {
                return 0;
        }

        if (list->length == 1) {
                free_node(list->start, free_value);
                *list = (struct List) {0, 0, 0};
                return 1;
        }

        struct Node * index_node;

        if (index == 0) {
                index_node = list->start;
                list->start = index_node->next;
                free_node(index_node, free_value);

        } else if (index == list->length - 1) {
                index_node = list_index_as_node(list, list->length - 2);
                list->end = index_node;
                free_node(index_node->next, free_value);
                index_node->next = 0;

        } else {
                index_node = list_index_as_node(list, index);
                list_index_as_node(list, index - 1)->next = list_index_as_node(list, index + 1);
                free_node(index_node, free_value);
        }

        --list->length;
        return 1;
}

struct List list_release_sublist(struct List * list, unsigned int start_index, unsigned int end_index)
{
        if (start_index >= end_index) {
                return (struct List) {0, 0, 0};
        }
        if (end_index > list->length) {
                return (struct List) {0, 0, 0};
        }

        // inits values in sublist
        struct List sublist = (struct List) {0, 0, 0};
        sublist.start = list_index_as_node(list, start_index);
        sublist.end = list_index_as_node(list, end_index - 1);

        if (end_index == list->length) {
                if (start_index == 0) {
                        list->end = 0;
                } else {
                        list->end = list_index_as_node(list, start_index - 1);
                }
        }
        // skips values in list
        if (start_index == 0) {
                list->start = list_index_as_node(list, end_index);
        } else {
                list_index_as_node(list, start_index - 1)->next = list_index_as_node(list, end_index);
        }

        // changes lengths of lists
        sublist.length = end_index - start_index;
        sublist.end->next = 0;
        list->length -= sublist.length;

        return sublist; // returns sublist
} // ends function
// previously empty line, now contains a comment

void list_delete(struct List * list, void (*free_value)(void *))
{
        while (list->length > 0) {
                list_pop(list, 0, free_value);
        }
}

void log_list(enum e_LogLevel log_level, struct List * list, void (* node_print_function)(enum e_LogLevel, void *), char * separator)
{
        for (struct Node * node = list->start; node; node = node->next) {
                node_print_function(log_level, node->value);

                if (node->next) {
                        logger(log_level, "%s", separator);
                }
        }
}
