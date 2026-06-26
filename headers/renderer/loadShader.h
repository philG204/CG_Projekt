/**
 * @file loadShader.h
 * @brief Shader file loading interface
 *
 * Provides functionality to load shader source code from files.
 */

#ifndef LOAD_SHADER_H
#define LOAD_SHADER_H

/**
 * @brief Load shader source code from a file
 *
 * Reads the entire contents of a shader file into dynamically allocated
 * memory. Useful for loading vertex or fragment shader source files.
 *
 * @param filename Path to the shader file
 * @return Pointer to null-terminated string containing shader source,
 *         or NULL if loading failed. Caller must free the returned pointer.
 */
char *loadShader (const char *filename);

#endif // LOAD_SHADER_H
