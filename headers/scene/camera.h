#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

typedef struct Pos
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Pos;

/*
    eye     - position
    center  - ziel
    up      - normalvektor
*/
typedef struct CameraSettings
{
  GLfloat eye[3];
  GLfloat center[3];
  GLfloat up[3];
} CameraSettings;

typedef struct ProjectionSettings
{
  GLfloat fovy;
  GLfloat aspect;
  GLfloat near_plane;
  GLfloat far_plane;
} ProjectionSettings;

/*
    view         -  saves where the camera is and whats it looking at
    projection   -  saves how the camera project 3d to 2d
    viewProj     -  for the shader gl_Position per matrix multiplication view
   and projection

    saves the CameraSetings and Projectionsettings for recalculation

    position     -  is the posiopn of the camera
    isDirty      -  check if recalculation is needed
*/
typedef struct Camera
{
  GLfloat view[16];
  GLfloat projection[16];
  GLfloat viewProj[16];

  CameraSettings settings;
  ProjectionSettings projectSettings;

  Pos position;
  int isDirty;
} Camera;

/*
    initasation of the Camera
*/
Camera *camera_init (CameraSettings *cameraSettings,
                     ProjectionSettings *projectionSettings);

/*
    Updates the view, projection and viewProj Matrix
    when they have change
    for the while loop
*/
void camera_update (Camera *camera);

/*
  Set the entire Camera Settings at once
*/
void camera_setCameraSettings (Camera *camera, CameraSettings *cameraSettings);

/*
  Set the entire Projections Settings at once
*/
void camera_setProjectionSettings (Camera *camera,
                                   ProjectionSettings *projectionSettings);

/*
  Set the Eye of the Camera
*/
void camera_setEye (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/*
  Set the Center of the Camera
*/
void camera_setCenter (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/*
  Set the Up/Normalvector of the Camera
*/
void camera_setUp (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/*
  Set the Fovy of the Camera
*/
void camera_setFovy (Camera *camera, GLfloat fovy);

/*
  Set the Aspect of the Camera
*/
void camera_setAspect (Camera *camera, GLfloat aspect);

/*
  Set the Near Plane of the Camera
*/
void camera_setNearPlane (Camera *camera, GLfloat near_plane);

/*
  Set the Far Plane of the Camera
*/
void camera_setFarPlane (Camera *camera, GLfloat far_plane);

#endif // CAMERA_H
