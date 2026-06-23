#ifndef MATRIX_TRANSFORMATION_H
#define MATRIX_TRANSFORMATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void inverse(GLfloat out, const GLfloat in);
void normalize (GLfloat *v);
void cross (const GLfloat *a, const GLfloat *b, GLfloat *out);
GLfloat dot (const GLfloat *a, const GLfloat *b);

void multiplyMatrices (GLfloat *out, const GLfloat *a, const GLfloat *b);

void identity (GLfloat *out);
void translate (GLfloat *out, const GLfloat *in, GLfloat *v);
void scale (GLfloat *out, const GLfloat *in, const GLfloat *v);

void rotatex (GLfloat *out, const GLfloat *in, GLfloat angle);
void rotatey (GLfloat *out, const GLfloat *in, GLfloat angle);
void rotatez (GLfloat *out, const GLfloat *in, GLfloat angle);

void lookAt (GLfloat *out, const GLfloat *eye, const GLfloat *center,
             const GLfloat *up);
void perspective (GLfloat *out, float fovy, float aspect, float near,
                  float far);

#endif // MATRIX_TRANSFORMATION_H
