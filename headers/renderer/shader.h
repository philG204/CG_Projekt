#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

/**
 *  @brief Initialize a pair of vertex and fragment shaders
 *
 *  @param shaderDir Base directory of the shader pair
 *  @return The programm handle
 */
GLuint shader_init (const char *shaderDir);

/**
 *  @brief Use a shader
 *
 *  @param shaderProgram The programm handle
 */
void use_shader (GLuint shaderProgram);

#endif // SHADER_H
