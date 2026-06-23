#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

GLuint shader_init(char* shaderDir);
void use_shader(GLuint shaderProgram);

#endif // SHADER_H
