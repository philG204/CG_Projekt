#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

/**
 *  @brief Get the filename of a file without extension
 *
 *  @param path
 *  @param out
 *  @param outSize
 */
void getNameWithoutExtension (const char *path, char *out, size_t outSize);

#endif // FILE_OPERATIONS_H
