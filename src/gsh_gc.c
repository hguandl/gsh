#include "gsh_gc.h"

void clean_char_arr(int len, char ***ptr) {
    for (int i = 0; i < len; ++i)
        _gc((*ptr)[i]);
    _gc(*ptr);
}
