#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

typedef struct Pos {
    GLfloat x,
    GLfloat y,
    GLfloat z,
} Pos;

typedef struct Camera{
    GLuint* viewProjMatrix,
    Pos position,
    GLfloat viewDirection
} Camera;

Camera* camera_init(const GLfloat eye[3], const GLfloat* center, const GLfloat* up, const float* fovy, const float* aspect, const float* near, const float* far);
void camera_update(Camera* camera);

#endif