#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stddef.h>

/**
 *  @brief
 *
 *  @param path
 *  @param out
 *  @param outSize
 */
void getNameWithoutExtension (const char *path, char *out, size_t outSize);

#endif // FILE_OPERATIONS_H
