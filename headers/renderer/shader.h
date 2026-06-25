#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

/**
 *  @brief
 *
 *  @param shaderDir
 *  @return
 */
GLuint shader_init (char *shaderDir);

/**
 *  @brief
 *
 *  @param shaderProgram
 */
void use_shader (GLuint shaderProgram);

#endif // SHADER_H
