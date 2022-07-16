#include "stack.h"

#define INDEX_OF_IP 0

char _push(struct Stack * stack)
{
        stack_element_t * elements_to_push = &stack->elements[(int) stack->elements[INDEX_OF_IP] + 2];
        stack_element_t amount_of_elements = stack->elements[(int) stack->elements[INDEX_OF_IP] + 1];

        if (!stack_push(stack, elements_to_push, amount_of_elements)) {
                return 0;
        }

        stack->elements[INDEX_OF_IP] += 1 + amount_of_elements;
        return 1;
}
