#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void 
getNameWithoutExtension(const char *path, char *out, size_t outSize);

#endif // FILE_OPERATIONS_H