// gets the content of files, directories etc
// the kind of file that doesn't necessarily
// need to be specific to this project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

static long file_length(FILE * fp)
{
        fseek(fp, 0, SEEK_END);
        unsigned long flength = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        return flength;
}

// converts file to null-terminated string
char * file_to_string(char * fpath)
{
        logger(LOG_INFO, "Moving the contents of \"%s\" to a string ...\n", fpath);

        FILE * fp = fopen(fpath, "r");
        if (!fp) {
                logger(LOG_ERROR, "Couldn't open \"%s\". Are you sure the file exists?\n", fpath);
                return 0;
        }

        long flength = file_length(fp);

        // on some operating systems, newlines takes 2 bytes of space
        // and it's hard to figure out what value flength really should
        // hold. but fread returns the amount of bytes read, so we can
        // get the correct flength no matter what
        char * fbuffer = malloc(flength + 1);
        long true_flength = fread(fbuffer, 1, flength, fp);
        fbuffer[true_flength] = 0;

        fclose(fp);

        // as explained, flength might be too long so fbuffer is larger than
        // what's needed. these two lines make sure no extra space is used
        char * fstring = malloc(strlen(fbuffer) + 1);
        strcpy(fstring, fbuffer);

        logger(LOG_DEBUG, "Contents of \"%s\":\n", fpath);
        logger(LOG_DEBUG, "%s\n\n", fstring);

        logger(LOG_SUCCESS, "Done moving \"%s\" to a string.\n", fpath);
        return fstring;
}

char * get_dirpath(char * fpath)
{
        char * last_backslash = strrchr(fpath, '\\');
        char * last_slash = strrchr(fpath, '/');
        if (last_backslash && last_slash) {
                logger(LOG_ERROR, "File path \"%s\" had both backslashes and slashes.\n");
        }

        // temporary null termination at the character after the last backslash or slash
        // so dirpath can be strcpy'd
        char * fname_start = (last_backslash ? last_backslash : last_slash) + 1;
        char first_char_in_fname = fname_start[0];
        fname_start[0] = 0;

        char * dirpath = malloc(strlen(fpath) + 1);
        strcpy(dirpath, fpath);

        fname_start[0] = first_char_in_fname;
        return dirpath;
}

char is_absolute_path(char * fpath)
{
        return strchr(fpath, ':') || fpath[0] == '\\' || fpath[0] == '/';
}
