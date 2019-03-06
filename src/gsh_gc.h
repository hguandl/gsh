#ifndef GSH_GC_H
#define GSH_GC_H 

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "readline/readline.h"

// Free a pointer safely
#define _gc(ptr) \
    if (ptr) { \
        free(ptr); \
        (ptr) = NULL; \
    }

/** Free each element in a char array,
    then free the pointer itself
* len - array length
* ptr - pointer to the head of array
*/
void clean_char_arr(int len, char ***ptr);

#endif  // GSH_CG_H
