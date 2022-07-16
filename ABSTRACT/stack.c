#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "log.h"
#include "instruction.h"

void log_stack_element_t(enum e_LogLevel log_level, void * stack_element)
{
        logger(log_level, g_stack_element_t_format, *(stack_element_t *) stack_element);
}

void log_as_instruction_and_stack_element_t(enum e_LogLevel log_level, void * value_to_log)
{
        log_instruction(log_level, value_to_log);
        logger(log_level, " (");
        log_stack_element_t(log_level, value_to_log);
        logger(log_level, ")");
}

struct Stack new_stack(void)
{
        struct Stack stack;
        stack.max_size = g_max_stack_size;
        stack.elements = malloc(sizeof(stack_element_t) * stack.max_size);
        stack.top = 0;
        return stack;
}

void log_stack(enum e_LogLevel log_level, struct Stack * stack, void (*log_value)(enum e_LogLevel, void *), char * separator)
{
        if (log_level < g_min_log_level) {
                return;
        }

        for (int i = 0; i <= stack->top; i++) {
                log_value(log_level, (void *) &stack->elements[i]);
                if (i != stack->top) {
                        logger(log_level, "%s", separator);
                }
        }
}

// makes stack g_max_stack_size elements longer
static void increase_stack_size(struct Stack * stack)
{
        logger(LOG_INFO, "Moving the elements of a stack onto a larger one ...\n");

        size_t curr_size = stack->max_size * sizeof(stack_element_t);
        size_t added_size = g_max_stack_size * sizeof(stack_element_t);
        stack_element_t * new_elements = malloc(curr_size + added_size);

        memcpy(new_elements, stack->elements, stack->max_size * sizeof(stack_element_t));
        free(stack->elements);
        stack->elements = new_elements;
        stack->max_size += g_max_stack_size;

        logger(LOG_SUCCESS, "Done moving the elements of a stack onto a larger one.\n");
}

char stack_push(struct Stack * stack, stack_element_t * values, stack_element_t amount)
{
        if (amount < 0) {
                logger(LOG_ERROR, "Negative element amount (%lf) for stack pushing.\n", amount);
                return 0;
        }

        size_t amount_as_size_t = (size_t) amount;
        if (amount != (stack_element_t) amount_as_size_t){
                logger(LOG_ERROR, "Non-integer element amount (%lf) for stack pushing.\n", amount);
                return 0;
        }

        while (stack->top + amount_as_size_t + 1 > stack->max_size) {
                increase_stack_size(stack);
        }

        memcpy(&stack->elements[stack->top + 1], values, amount_as_size_t * sizeof(stack_element_t));
        stack->top += amount;
        return 1;
}

char stack_pop(struct Stack * stack, stack_element_t amount)
{
        if (amount < 0) {
                logger(LOG_ERROR, "Negative element amount (%lf) for stack popping.\n", amount);
                return 0;
        }

        size_t amount_as_size_t = (size_t) amount;
        if (amount != (stack_element_t) amount_as_size_t){
                logger(LOG_ERROR, "Non-integer element amount (%lf) for stack popping.\n", amount);
                return 0;
        }

        if (stack->top < amount_as_size_t) {
                logger(LOG_ERROR, "Trying to pop more elements (%u) than there are in a stack.\n", amount_as_size_t);
        }

        stack->top -= amount_as_size_t;
        return 1;
}
