#include <string.h>
#include <stdlib.h>

#include "gsh_gc.h"
#include "gsh_parser.h"

char *trim(char *text) {
    int len = strlen(text);
    int start, end;
    for (start = 0; start < len; ++start)
        if (text[start] != ' ' && text[start] != '\n')
            break;
    for (end = len - 1; end >= 0; --end)
        if (text[end] != ' ' && text[end] != '\n')
            break;

    char *new_text = text + start;
    *(text + end + 1) = '\0';

    return new_text;
}

static int escape_quote(char esc, char **p, char **arg_s, char **arg_t) {
    ++(*arg_s);
    ++(*p);
    while (**p != esc) {
        // No matched quote
        if (**p == '\0')
            return -1;
        ++(*p);
    }
    *arg_t = *p;
    ++(*p);
    return 0;
}

int parse_args(char *line, char **path_ptr, int *argl, char ***args_ptr) {
    char *s = line;
    char *p = s;
    // Find the end of exec path
    while (*p != ' ' && *p != '\0')
        ++p;
    // Setup exec path
    char *path = (char *)malloc((p - s + 1) * sizeof(char));
    strncpy(path, s, p - s);
    path[p - s] = '\0';

    // Initalize argument array with length of 8
    int buf_size = 8;
    *argl = 0;
    char **args = (char **)malloc((buf_size) * sizeof(char *));
    p = s;  // Back to the beginning
    while (*p != '\0') {
        char *arg_s;  // start position of current argument
        char *arg_t;  // end position of current argument
        while (*p == ' ' && *p != '\0')
            ++p;
        arg_s = p;
        // Handle quotes
        if (*p == '"') {
            if (escape_quote('"', &p, &arg_s, &arg_t)) {
                // Error, cleanup and return
                clean_char_arr(*argl, &args);
                free(path);
                return -1;
            }
        } else if (*p == '\'') {
            if (escape_quote('\'', &p, &arg_s, &arg_t)) {
                clean_char_arr(*argl, &args);
                free(path);
                return -1;
            }
        } else {
            while (*p != ' ' && *p != '\0') 
                ++p;
            arg_t = p;
        }
        ++(*argl);  // Get a new argument
        char *arg = (char *)malloc((arg_t - arg_s + 1) * sizeof(char));
        strncpy(arg, arg_s, arg_t - arg_s);
        arg[arg_t - arg_s] = '\0';
        args[*argl - 1] = arg;
        // Resize when the length is not enough
        if (*argl >= buf_size) {
            buf_size <<= 1;
            args = (char **)realloc(args, (buf_size) * sizeof(char *));
        }
    }
    args[*argl] = NULL;  // Argument list should end with NULL
    *path_ptr = path;
    *args_ptr = args;
    return 0;
}
