#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void identity(GLfloat *out);
void translate(GLfloat *out, const GLfloat *in, GLfloat *v);
void scale(GLfloat *out, const GLfloat *in, const GLfloat *v);

void rotatey(GLfloat *out, const GLfloat *in, GLfloat angle);
void rotatez(GLfloat *out, const GLfloat *in, GLfloat angle);

void lookAt(GLfloat *out, const GLfloat *eye, const GLfloat *center, const GLfloat *up);
void perspective(GLfloat *out, float fovy, float aspect, float near, float far);