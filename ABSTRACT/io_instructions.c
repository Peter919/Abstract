#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "settings.h"

#define MAX_INPUT_SIZE 10000

char _input_string(struct Stack * stack)
{
        char * str = malloc(MAX_INPUT_SIZE);
        fgets(str, MAX_INPUT_SIZE, stdin);

        size_t buffer_size = strlen(str) + 1;

        stack_element_t * stack_str;
        stack_str = malloc(buffer_size * sizeof(stack_element_t));
        for (size_t i = 0; i < buffer_size; i++) {
                stack_str[i] = str[i];
        }

        free(str);
        stack_push(stack, stack_str, buffer_size);
        free(stack_str);
        return 1;
}

char _output_string(struct Stack * stack)
{
        unsigned int printing_start = (unsigned int) stack->elements[stack->top];
        if (printing_start != stack->elements[stack->top]) {
                logger(LOG_ERROR, "Negative or non-integer address of output string.\n");
        }
        if (printing_start > stack->top || printing_start < 0) {
                logger(LOG_ERROR, "Address of output string out of stack bounds.\n");
                return 0;
        }

        for (unsigned int i = printing_start; stack->elements[i]; i++)
        {
                putchar((int) stack->elements[i]);

                if (i > stack->top) {
                        logger(LOG_ERROR, "No null-termination for output string.\n");
                        return 0;
                }
        }

        return stack_pop(stack, 1);
}

char _input_number(struct Stack * stack)
{
        stack_element_t * num = alloca(sizeof(stack_element_t));
        scanf(g_stack_element_t_format, num);
        stack_push(stack, num, 1);

        return 1;
}

char _output_number(struct Stack * stack)
{
        printf(g_stack_element_t_format, stack->elements[stack->top]);
        return stack_pop(stack, 1);
}
