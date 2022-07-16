#include <string.h>
#include "settings.h"
#include "token.h"

char is_whitespace(char ch)
{
        switch (ch) {
        case ' ':
        case '\n':
        case '\t':
        case '\r':
                return 1;
        default:
                return 0;
        }
}

char is_prefix(char * pre, char * str)
{
        for (int i = 0; i < strlen(pre); i++) {
                if (pre[i] != str[i]) {
                        return 0;
                }
        }
        return 1;
}

char is_alphabetic(char ch)
{
        if (!ch) {
                return 0;
        }
        if (strchr(g_primary_alphabet, ch)) {
                return 2;
        }
        if (strchr(g_secondary_alphabet, ch)) {
                return 1;
        }
        return 0;
}

// negative numbers ends with a _
char is_number(char * str)
{
        char * number_start = str;
        if (*number_start == '-') {
                ++number_start;
        }
        if (*number_start == '.') {
                ++number_start;
        }
        if (*number_start && strchr("0123456789", *number_start)) {
                return 1;
        }
        return 0;
}

// checks if word is a prefix for str. if the last character
// in word is alphabetical, the character after that in str
// must be non-alphabetical because otherwise it's just an
// identifier (so f*oo is not the first word of f*oobar
// since oobar is treated as an identifier, but foo* is the
// first word in foo*bar)
// empty word returns 0
char is_first_word(char * word, char * str)
{
        if (!word[0]) {
                return 0;
        }
        if (!is_prefix(word, str)) {
                return 0;
        }

        size_t word_length = strlen(word);

        if (!is_alphabetic(str[word_length - 1])) {
                return 1;
        }
        if (!is_alphabetic(str[word_length])) {
                return 1;
        }
        return 0;
}

int index_of_first_word(char ** word_array, unsigned int word_list_length, char * str)
{
        for (int index = 0; index < word_list_length; index++) {
                char * curr_word = word_array[index];
                if (is_first_word(curr_word, str)) {
                        return index;
                }
        }

        return -1;
}
