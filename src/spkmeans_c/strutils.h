#ifndef STRUTILS_H
#define STRUTILS_H

#include <stddef.h>

/**
 * Receive a string and a character, and count the occurences of the given
 * character in the string.
 */
size_t strcount(char *str, char c);

#endif
