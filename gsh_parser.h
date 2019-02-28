#ifndef GSH_PARSER_H
#define GSH_PARSER_H

/** Trim space and LF on both sides
* text    - original string
* @return - pointer to trimmed string
*/
char *trim(char *text);

/** Handle contents between quotes
* esc     - qoute char
* p       - cursor pointer
* arg_s   - start position of current argument
* arg_t   - end position of current argument
* @return - 0 if succeed
*/
static int escape_quote(char esc, char **p, char **arg_s, char **arg_t);

/** Parse command to get
* line     - original command
* path_ptr - execution path 
* argl     - argument array length
* args_ptr - argument array
* @return  - 0 if succeed
*/
int parse_args(char *line, char **path_ptr, int *argl, char ***args_ptr);

#endif  // GSH_PARSER_H
