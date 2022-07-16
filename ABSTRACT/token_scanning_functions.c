#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "token_recognition_functions.h"
#include "buffer.h"

#define EXPECTED_CHAR_BEFORE_EOF(ch) \
        logger(LOG_ERROR, "Expected \'%c\' before EOF in \"%s\".\n", ch, fpath);

char scan_comment(char ** scanning_pos, char * fpath)
{
        unsigned int depth = 1;
        do {
                ++*scanning_pos;

                if (**scanning_pos == '(') {
                        ++depth;
                } else if (**scanning_pos == ')') {
                        --depth;
                }

                if (!**scanning_pos) {
                        EXPECTED_CHAR_BEFORE_EOF(')');
                        return 0;
                }
        } while (depth > 0);
        // skip closing bracket
        ++*scanning_pos;

        return 1;
}

char * scan_file(char ** scanning_pos, char * fpath)
{
        size_t chars_to_copy = 0;
        // skip opening bracket
        ++*scanning_pos;
        char * curr_char = *scanning_pos;

        while (*curr_char != ']') {
                ++curr_char;
                ++chars_to_copy;

                if (!*curr_char) {
                        EXPECTED_CHAR_BEFORE_EOF(']');
                        return 0;
                }
        }

        char * fname = malloc(chars_to_copy + 1);
        // skip opening bracket by adding 1
        memcpy(fname, *scanning_pos, chars_to_copy);
        fname[chars_to_copy] = 0;

        // skip closing bracket
        *scanning_pos += chars_to_copy + 1;

        return fname;
}

char * scan_identifier(char ** scanning_pos)
{
        // it's guaranteed to be at least 1 character long. if it isn't, it's
        // not an identifier, and that means it's get_token_type that returned
        // the wrong value, not this function
        size_t identifier_length = 1;
        while (is_alphabetic((*scanning_pos)[identifier_length])) {
                if (!(*scanning_pos)[identifier_length]) {
                        break;
                }
                ++identifier_length;
        }

        char * identifier = malloc(identifier_length + 1);
        memcpy(identifier, *scanning_pos, identifier_length);
        identifier[identifier_length] = 0;

        *scanning_pos += identifier_length;

        return identifier;
}

// is just used for approximate malloc length, so complicated
// error checking and escape codes aren't needed yet
static size_t string_length(char * scanning_pos)
{
        size_t length = 0;

        char * curr_char = scanning_pos + 1;
        while (*curr_char != '\"' || *(curr_char - 1) != '\\') {
                ++length;
                ++curr_char;

                if (!*curr_char) {
                        break;
                }
        }
        return length;
}

static char scan_char_from_string(char ** scanning_pos, char ** writing_pos, unsigned int * line)
{
        if (!**scanning_pos || **scanning_pos == '\n') {
                logger(LOG_ERROR, "Unclosed quotes or double quotes.\n");
                return 0;
        }
        if (**scanning_pos != '\\') {
                **writing_pos = **scanning_pos;
                ++*writing_pos;
                ++*scanning_pos;
                return 1;
        }

        switch ((*scanning_pos)[1]) {
        case '\"':
        case '\'':
        case '\\':
                **writing_pos = (*scanning_pos)[1];
                break;
        case 'n':
                **writing_pos = '\n';
                break;
        case 't':
                **writing_pos = '\t';
                break;
        case 'r':
                **writing_pos = '\r';
                break;
        case '0':
                **writing_pos = '\0';
                break;
        case '\n':
                ++*line;
                // reverses ++*writing_pos below since we don't write anything
                --*writing_pos;
                break;
        default:
                logger(LOG_WARNING, "Unknown escape sequence \'%c%c\'.\n", **scanning_pos, (*scanning_pos)[1]);
                **writing_pos = **scanning_pos;

                // cancels one incrementation of *scanning_pos += 2 below so both characters
                // in the escape sequence are written to the string buffer
                --*scanning_pos;
        }
        *scanning_pos += 2;
        ++*writing_pos;
        return 1;
}

struct Buffer * scan_string(char ** scanning_pos, unsigned int * line, char is_characters)
{
        char * str = malloc(string_length(*scanning_pos) + 1);
        char * writing_pos = str;
        // skip opening quotes
        ++*scanning_pos;

        while (**scanning_pos != (is_characters ? '\'' : '\"')) {
                if (!scan_char_from_string(scanning_pos, &writing_pos, line)) {
                        return 0;
                }
        }
        if (!is_characters) {
                *writing_pos++ = 0;
        }
        // skip closing quotes
        ++*scanning_pos;

        struct Buffer * buffer = malloc(sizeof(struct Buffer));
        buffer->size = (size_t) (writing_pos - str);
        buffer->contents = malloc(buffer->size);
        memcpy(buffer->contents, str, buffer->size);
        free(str);

        return buffer;
}

double * scan_number(char ** scanning_pos)
{
        double * num = malloc(sizeof(double));
        char * num_start = *scanning_pos;
        *num = strtod(num_start, scanning_pos);

        // decimal separator in the end of a number isn't allowed
        if (*(*scanning_pos - 1) == '.') {
                --*scanning_pos;
        }

        return num;
}
