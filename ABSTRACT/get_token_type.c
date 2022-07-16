#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "token.h"
#include "token_recognition_functions.h"

#define POINTER_OPERATORS_START TOK_STACK_TOP_ADDRESS
#define MATH_OPERATORS_START TOK_ADD
#define IO_OPERATORS_START TOK_INPUT_STRING
#define FILE_OPERATORS_START TOK_FILE_READ

static char * tok_to_word(enum e_Token token)
{
        switch (token) {
        case TOK_COMMENT: return "(";
        case TOK_FILE: return "[";
        case TOK_EXIT: return "~";
        case TOK_STRING: return "\"";
        case TOK_CHARACTERS: return "\'";
        case TOK_INSTRUCTIONS_START: return "{";
        case TOK_INSTRUCTIONS_END: return "}";
        case TOK_INSTRUCTION_POINTER: return "ip";
        case TOK_STACK_TOP_ADDRESS: return "&";
        case TOK_SET_ADDRESS: return "!";
        case TOK_GET_ADDRESS: return "@";
        case TOK_ADD: return "+";
        case TOK_SUBTRACT: return "-";
        case TOK_MULTIPLY: return "*";
        case TOK_DIVIDE: return "/";
        case TOK_MODULO: return "%";
        case TOK_POWER: return "^";
        case TOK_ROOT: return "root";
        case TOK_LOGARITHM: return "log";
        case TOK_SINE: return "sin";
        case TOK_COSINE: return "cos";
        case TOK_TANGENT: return "tan";
        case TOK_ARCSINE: return "asin";
        case TOK_ARCCOSINE: return "acos";
        case TOK_ARCTANGENT: return "atan";
        case TOK_ABSOLUTE_VALUE: return "abs";
        case TOK_ROUND: return "round";
        case TOK_FLOOR: return "floor";
        case TOK_CEILING: return "ceil";
        case TOK_INPUT_STRING: return ",";
        case TOK_OUTPUT_STRING: return ".";
        case TOK_INPUT_NUMBER: return ";";
        case TOK_OUTPUT_NUMBER: return ":";
        // might add these later, but for now they aren't recognized
        // case TOK_FILE_READ: return "filer";
        // case TOK_FILE_WRITE: return "filew";
        // case TOK_FILE_READ_BINARY: return "filerb";
        // case TOK_FILE_WRITE_BINARY: return "filewb";
        default: return "";
        }
}

// list of words used for tokens (like "+" for addition and "filer" for file read)
// all the elements correspond to an e_Token enumeration value, so
// get_token_words()[TOK_ADD] returns "+"
static char ** get_token_words(void)
{
        char ** token_words = malloc(sizeof(char *) * TOK_AMOUNT_OF_STANDARD_TOKENS);
        char * curr_word;

        for (enum e_Token tok = 0; tok < TOK_AMOUNT_OF_STANDARD_TOKENS; tok++) {
                curr_word = tok_to_word(tok);
                token_words[tok] = malloc(strlen(curr_word) + 1);
                strcpy(token_words[tok], curr_word);
        }

        return token_words;
}

char get_token_type(char * scanning_pos)
{
        // most tokens simply need to start with a specific string, so this
        // saves a lot of lines of code
        static char ** token_words = 0;
        if (!token_words) {
                token_words = get_token_words();
        }

        // must be checked before TOK_SUBTRACT so negative numbers work
        if (is_number(scanning_pos)) {
                return TOK_NUMBER;
        }

        // since the elements in token_words correspond to e_Token values, this will
        // return the enumeration value that the first word in word_index belongs
        // to
        int word_index = index_of_first_word(token_words, TOK_AMOUNT_OF_STANDARD_TOKENS, scanning_pos);
        if (word_index != -1) {
                return word_index;
        }

        if (is_whitespace(*scanning_pos)) {
                return TOK_WHITESPACE;
        }
        if (is_alphabetic(*scanning_pos) == 2) {
                return TOK_IDENTIFIER;
        }

        return TOK_INVALID;
}
