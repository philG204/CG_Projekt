#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

typedef struct Pos {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Pos;

typedef struct CameraSettings {
    GLfloat eye[3];
    GLfloat center[3];
    GLfloat up[3];
} CameraSettings;

typedef struct ProjectionSettings {
    GLfloat fovy;
    GLfloat aspect;
    GLfloat near_plane;
    GLfloat far_plane;
} ProjectionSettings;

typedef struct Camera {
    GLfloat* viewProjMatrix;
    Pos position;
    GLfloat viewDirection;
} Camera;

Camera* camera_init(CameraSettings* cameraSettings, ProjectionSettings* projectionSettings);
void camera_update(Camera* camera);

#endif // CAMERA_H
