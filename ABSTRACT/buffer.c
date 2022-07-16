#include "buffer.h"
#include <stdlib.h>
#include "log.h"

void free_buffer(void * buffer)
{
        struct Buffer * bufptr = (struct Buffer *) buffer;
        free(bufptr->contents);
        free(bufptr);
}

void log_buffer_as_chars(enum e_LogLevel log_level, void * buffer)
{
        struct Buffer * bufptr = (struct Buffer *) buffer;
        for (size_t i = 0; i < bufptr->size; i++) {
                logger(log_level, "%c", bufptr->contents[i]);
        }
}
