#pragma once

char scan_comment(char ** scanning_pos, char * fpath);

char * scan_file(char ** scanning_pos, char * fpath);

char * scan_identifier(char ** scanning_pos);

struct Buffer * scan_string(char ** scanning_pos, unsigned int * line, char is_characters);

double * scan_number(char ** scanning_pos);
