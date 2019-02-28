#ifndef GSH_H
#define GSH_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "readline/readline.h"

#define _gc(ptr) \
    if (ptr) { \
        free(ptr); \
        (ptr) = NULL; \
    }

#endif  // GSH_H