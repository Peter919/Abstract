#pragma once

char is_whitespace(char ch);

char is_prefix(char * pre, char * str);

char is_alphabetic(char ch);

char is_number(char * str);

char is_first_word(char * word, char * str);

int index_of_first_word(char ** word_array, unsigned int word_list_length, char * str);
