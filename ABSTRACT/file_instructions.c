// not at all ready, might work on it later

#include "stack.h"
#include <string.h>
#include "file_handling.h"

#define MAX_FNAME_LENGTH 10000

static inline char * get_file_name_from_stack(struct Stack * stack, unsigned int address)
{
        if (address < 0) {
                logger(LOG_ERROR, "Cannot get a file name from a negative address.\n");
                return 0;
        }

        char * fname = malloc(MAX_FNAME_LENGTH + 1);
        unsigned int index = 0;
        for (index = 0; stack->elements[address + index]; index++) {
                if (index > MAX_FNAME_LENGTH) {
                        logger(LOG_ERROR, "Cannot accept file names longer than %u bytes.\n", MAX_FNAME_LENGTH);
                        return 0;
                }

                fname[index] = stack->elements[address + index];
        }
        fname[index] = 0;
        return fname;
}

static inline stack_element_t * str_to_stack_elements(char * str)
{
        size_t str_length = strlen(str) + 1;
        stack_element_t * elements = malloc(str_length * sizeof(stack_element_t));

        for (size_t i = 0; i <= str_length; i++) {
                elements[i] = str[i];
        }
        return elements;
}

char _file_read(struct Stack * stack)
{
        char * fname = get_file_name_from_stack(stack, stack->top);
        if (!fname) {
                return 0;
        }

        char * fcontents_as_str = file_to_string(fname);
        free(fname);
        stack_element_t * fcontents = str_to_stack_elements(fcontents_as_str);

        stack_push(stack, fcontents, strlen(fcontents_as_str) + 1);
        free(fcontents_as_str);

        stack_pop(stack, 1);
        return 1;
}

char _file_write(struct Stack * stack){return 1;}

char _file_read_binary(struct Stack * stack){return 1;}

char _file_write_binary(struct Stack * stack){return 1;}
