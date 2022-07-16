#include "stack.h"
#include <stdlib.h>

#define INDEX_OF_IP 0

char _stack_top_address(struct Stack * stack)
{
        stack_element_t * address = alloca(sizeof(stack_element_t));
        *address = stack->top;
        return stack_push(stack, address, 1);
}

char _set_address(struct Stack * stack)
{
        unsigned int address = stack->elements[stack->top - 1];
        if (stack->elements[stack->top - 1] != address) {
                logger(LOG_ERROR, "Cannot assign a value to a negative or non-integer address.\n");
                return 0;
        }
        if (address > stack->top || address < 0) {
                logger(LOG_ERROR, "Cannot assign a value to an address outside of stack bounds.\n");
                return 0;
        }

        stack_element_t value = stack->elements[stack->top];

        stack->elements[address] = value;
        stack_pop(stack, 2);

        // reverses ++stack->elements[INDEX_OF_IP]; that will be executed
        // after this function as returned
        if (address == INDEX_OF_IP) {
                --stack->elements[INDEX_OF_IP];
        }

        return 1;
}

char _get_address(struct Stack * stack)
{
        unsigned int address = stack->elements[stack->top];
        if (address != stack->elements[stack->top]) {
                logger(LOG_ERROR, "Cannot get a value from a negative or non-integer address.\n");
                return 0;
        }
        if (address > stack->top || address < 0) {
                logger(LOG_ERROR, "Cannot get a value from an address outside of stack bounds.\n");
                return 0;
        }

        stack->elements[stack->top] = stack->elements[address];
        return 1;
}
