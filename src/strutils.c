#include <string.h>
#include "strutils.h"


size_t strcount(char *str, char c) {
    size_t length = strlen(str), count = 0, i;
    for (i = 0; i < length; i++) {
        if (str[i] == c) {
            count++;
        }
    }
    return count;
}
