#pragma once
#include <stdlib.h>
#include "log.h"

struct Buffer
{
        size_t size;
        char * contents;
};

void free_buffer(void * buffer);

void log_buffer_as_chars(enum e_LogLevel log_level, void * buffer);
