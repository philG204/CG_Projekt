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

/**

  @param view         -  saves where the camera is and whats it looking at
  @param projection   -  saves how the camera project 3d to 2d
  @param viewProj     -  for the shader gl_Position per matrix multiplication
  view and projection

  @param settings saves the CameraSetings and Projectionsettings for
  recalculation
  @param projectionSettings

  @param position     -  is the posiopn of the camera
  @param isDirty      -  check if recalculation is needed
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

/**
 *  @brief
 *
 *  @param cameraSettings A Pointer to a deklaration Variable of the struct
 * Camerasettings
 *  @param projectionSettings A Pointer to a deklaration Variable of the struct
 * Projections
 *  @return returns a struct of a Camera
 */
Camera *camera_init (CameraSettings *cameraSettings,
                     ProjectionSettings *projectionSettings);

/**
 *  @brief Updates the Camera and updates the Matricies
  Updates the Camera and when the something has change recalculates the
 Matricies
 *
 *  @param camera pointer to a Camera
*/
void camera_update (Camera *camera);

/**
 *  @brief Set the entire Camera Settings at once
 *
 *  @param camera
 *  @param cameraSettings
 */
void camera_setCameraSettings (Camera *camera, CameraSettings *cameraSettings);

/**
 *  @brief Set the entire Projections Settings at once
 *
 *  @param camera
 *  @param projectionSettings
 */
void camera_setProjectionSettings (Camera *camera,
                                   ProjectionSettings *projectionSettings);

/**
 *  @brief Set the Eye of the Camera
 *
 *  @param camera
 *  @param x
 *  @param y
 *  @param z
 */
void camera_setEye (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/**
 *  @brief Set the Center of the Camera
 *
 *  @param camera
 *  @param x
 *  @param y
 *  @param z
 */
void camera_setCenter (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/**
 *  @brief Set the Up/Normalvector of the Camera
 *
 *  @param camera
 *  @param x
 *  @param y
 *  @param z
 */
void camera_setUp (Camera *camera, GLfloat x, GLfloat y, GLfloat z);

/**
 *  @brief Set the Fovy of the Camera
 *
 *  @param camera
 *  @param fovy
 */
void camera_setFovy (Camera *camera, GLfloat fovy);

/**
 *  @brief Set the Aspect of the Camera
 *
 *  @param camera
 *  @param aspect
 */
void camera_setAspect (Camera *camera, GLfloat aspect);

/**
 *  @brief Set the Near Plane of the Camera

 *  @param camera
 *  @param near_plane
*/
void camera_setNearPlane (Camera *camera, GLfloat near_plane);

/**
 *  @brief Set the Far Plane of the Camera
 *
 *  @param camera
 *  @param far_plane
 */
void camera_setFarPlane (Camera *camera, GLfloat far_plane);

#endif // CAMERA_H
