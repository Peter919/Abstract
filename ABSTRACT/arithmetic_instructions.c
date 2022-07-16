#include "stack.h"
#include "math.h"

static inline char arithmetic_function(struct Stack * stack, unsigned int args_to_pop, stack_element_t res)
{
        if (stack->top < args_to_pop) {
                logger(LOG_ERROR, "Trying to pop more elements than there are in the stack.\n");
                return 0;
        }

        // -1 because res should be pushed
        stack->top -= args_to_pop - 1;
        stack->elements[stack->top] = res;
        return 1;
}

char _add(struct Stack * stack)
{
        stack_element_t res = stack->elements[stack->top - 1] + stack->elements[stack->top];
        return arithmetic_function(stack, 2, res);
}

char _subtract(struct Stack * stack)
{
        stack_element_t res = stack->elements[stack->top - 1] - stack->elements[stack->top];
        return arithmetic_function(stack, 2, res);
}

char _multiply(struct Stack * stack)
{
        stack_element_t res = stack->elements[stack->top - 1] * stack->elements[stack->top];
        return arithmetic_function(stack, 2, res);
}

char _divide(struct Stack * stack)
{
        if (stack->elements[stack->top] == 0) {
                logger(LOG_ERROR, "Division by 0.\n");
                return 0;
        }

        stack_element_t res = stack->elements[stack->top - 1] / stack->elements[stack->top];
        return arithmetic_function(stack, 2, res);
}

char _modulo(struct Stack * stack)
{
        if (stack->elements[stack->top] == 0) {
                logger(LOG_ERROR, "Modulo 0.\n");
                return 0;
        }

        stack_element_t res = fmod(stack->elements[stack->top - 1], stack->elements[stack->top]);
        return arithmetic_function(stack, 2, res);
}

char _power(struct Stack * stack)
{
        if (stack->elements[stack->top - 1] == 0 && stack->elements[stack->top] == 0) {
                logger(LOG_ERROR, "0 to the power of 0.\n");
                return 0;
        }

        stack_element_t res = pow(stack->elements[stack->top - 1], stack->elements[stack->top]);
        return arithmetic_function(stack, 2, res);
}

char _root(struct Stack * stack)
{
        if (stack->elements[stack->top] == 0) {
                logger(LOG_ERROR, "Attempt to calculate 0th root.\n");
                return 0;
        }

        stack_element_t res = pow(stack->elements[stack->top], 1.0 / stack->elements[stack->top - 1]);
        return arithmetic_function(stack, 2, res);
}

char _logarithm(struct Stack * stack)
{
        if (stack->elements[stack->top - 1] <= 0) {
                logger(LOG_ERROR, "Logarithm base less than or equal to 0.\n");
                return 0;
        }
        if (stack->elements[stack->top - 1] == 1) {
                logger(LOG_ERROR, "Logarithm base equals 1.\n");
                return 0;
        }

        if (stack->elements[stack->top - 1] <= 0) {
                logger(LOG_ERROR, "Logarithm argument less than or equal to 0.\n");
                return 0;
        }

        stack_element_t res = log(stack->elements[stack->top]) / log(stack->elements[stack->top - 1]);
        return arithmetic_function(stack, 2, res);
}

char _sine(struct Stack * stack)
{
        stack_element_t res = sin(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _cosine(struct Stack * stack)
{
        stack_element_t res = cos(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _tangent(struct Stack * stack)
{
        if (fmod(stack->elements[stack->top], M_PI) == M_PI / 2) {
                logger(LOG_ERROR, "Undefined tangent.\n");
                return 0;
        }

        stack_element_t res = tan(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _arcsine(struct Stack * stack)
{
        if (stack->elements[stack->top] < -1 || stack->elements[stack->top] > 1) {
                logger(LOG_ERROR, "Argument for arcsine not in range [-1, 1].\n");
                return 0;
        }

        stack_element_t res = asin(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _arccosine(struct Stack * stack)
{
        if (stack->elements[stack->top] < -1 || stack->elements[stack->top] > 1) {
                logger(LOG_ERROR, "Argument for arccosine not in range [-1, 1].\n");
                return 0;
        }

        stack_element_t res = acos(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _arctangent(struct Stack * stack)
{
        stack_element_t res = atan(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _absolute_value(struct Stack * stack)
{
        stack_element_t res = fabs(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _round(struct Stack * stack)
{
        stack_element_t res = round(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _floor(struct Stack * stack)
{
        stack_element_t res = floor(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}

char _ceiling(struct Stack * stack)
{
        stack_element_t res = ceil(stack->elements[stack->top]);
        return arithmetic_function(stack, 1, res);
}
