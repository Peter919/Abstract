#pragma once
#include "settings.h"

struct Stack
{
        stack_element_t * elements;
        size_t max_size;
        size_t top;
};

void log_stack_element_t(enum e_LogLevel, void * stack_element);

void log_as_instruction_and_stack_element_t(enum e_LogLevel log_level, void * value_to_log);

struct Stack new_stack(void);

void log_stack(enum e_LogLevel log_level, struct Stack * stack, void (*log_value)(enum e_LogLevel, void *), char * separator);

char stack_push(struct Stack * stack, stack_element_t * values, stack_element_t amount);

char stack_pop(struct Stack * stack, stack_element_t amount);
