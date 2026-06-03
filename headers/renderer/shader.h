#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

GLuint* shader_init(char* vertexShader, char* fragmentShader);
void use_shader(GLuint* shaderProgram);

#endif