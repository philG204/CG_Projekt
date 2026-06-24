#include <assert.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/camera.h"

/**
  @brief

  @param cameraSettings A Pointer to a deklaration Variable of the struct Camerasettings
  @param projectionSettings A Pointer to a deklaration Variable of the struct Projections
  @return returns a struct of a Camera
*/
Camera *
camera_init (CameraSettings *cameraSettings,
             ProjectionSettings *projectionSettings)
{
  assert (cameraSettings != NULL);
  assert (projectionSettings != NULL);

  Camera *camera = malloc (sizeof (Camera));
  Pos position = { .x = cameraSettings->eye[0],
                   .y = cameraSettings->eye[1],
                   .z = cameraSettings->eye[2] };

  // Copy the camera and Projection settings into the camera
  memcpy (&camera->settings, cameraSettings, sizeof (CameraSettings));
  memcpy (&camera->projectSettings, projectionSettings,
          sizeof (ProjectionSettings));

  lookAt (camera->view, cameraSettings->eye, cameraSettings->center,
          cameraSettings->up);
  perspective (camera->projection, projectionSettings->fovy,
               projectionSettings->aspect, projectionSettings->near_plane,
               projectionSettings->far_plane);

  multiplyMatrices (camera->viewProj, camera->projection, camera->view);

  // Set the Position of the Camera
  camera->position = position;

  // Recalculate Flag set to false
  camera->isDirty = 0;

  return camera;
}

/**
  @brief Updates the Camera and updates the Matricies
  Updates the Camera and when the something has change recalculates the Matricies

  @param camera pointer to a Camera
*/
void
camera_update (Camera *camera)
{
  assert (camera != NULL);

  // nothing has change no recalculation needed
  if (camera->isDirty == 0)
    {
      return;
    }

  // Recalculate the Matrixes
  lookAt (camera->view, camera->settings.eye, camera->settings.center,
          camera->settings.up);
  perspective (camera->projection, camera->projectSettings.fovy,
               camera->projectSettings.aspect,
               camera->projectSettings.near_plane,
               camera->projectSettings.far_plane);

  multiplyMatrices (camera->viewProj, camera->projection, camera->view);

  // Update position
  camera->position.x = camera->settings.eye[0];
  camera->position.y = camera->settings.eye[1];
  camera->position.z = camera->settings.eye[2];

  // Clear recalculate flag
  camera->isDirty = 0;
}

/**
  @brief Set the entire Camera Settings at once

  @param camera
  @param cameraSettings
*/
void
camera_setCameraSettings (Camera *camera, CameraSettings *cameraSettings)
{
  assert (camera != NULL);
  assert (cameraSettings != NULL);

  if (memcmp (&camera->settings, cameraSettings, sizeof (CameraSettings)) != 0)
    {
      memcpy (&camera->settings, cameraSettings, sizeof (CameraSettings));
      camera->isDirty = 1;
    }
}

/**
  @brief Set the entire Projections Settings at once

  @param camera
  @param projectionSettings
*/
void
camera_setProjectionSettings (Camera *camera,
                              ProjectionSettings *projectionSettings)
{
  assert (camera != NULL);
  assert (projectionSettings != NULL);

  if (memcmp (&camera->settings, projectionSettings,
              sizeof (ProjectionSettings))
      != 0)
    {
      memcpy (&camera->settings, projectionSettings,
              sizeof (ProjectionSettings));
      camera->isDirty = 1;
    }
}

/**
  @brief Set the Eye of the Camera

  @param camera
  @param x
  @param y
  @param z
*/
void
camera_setEye (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  assert (camera != NULL);

  camera->settings.eye[0] = x;
  camera->settings.eye[1] = y;
  camera->settings.eye[2] = z;
  camera->isDirty = 1;
}

/**
  @brief Set the Center of the Camera

  @param camera
  @param x
  @param y
  @param z
*/
void
camera_setCenter (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  assert (camera != NULL);

  camera->settings.center[0] = x;
  camera->settings.center[1] = y;
  camera->settings.center[2] = z;
  camera->isDirty = 1;
}

/**
  @brief Set the Up of the Camera

  @param camera
  @param x
  @param y
  @param z
*/
void
camera_setUp (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  assert (camera != NULL);

  camera->settings.up[0] = x;
  camera->settings.up[1] = y;
  camera->settings.up[2] = z;
  camera->isDirty = 1;
}

/**
  @brief Set the Fovy of the Camera

  @param camera
  @param fovy
*/
void
camera_setFovy (Camera *camera, GLfloat fovy)
{
  assert (camera != NULL);

  camera->projectSettings.fovy = fovy;
  camera->isDirty = 1;
}

/**
  @brief Set the Aspect of the Camera

  @param camera
  @param aspect
*/
void
camera_setAspect (Camera *camera, GLfloat aspect)
{
  assert (camera != NULL);

  camera->projectSettings.aspect = aspect;
  camera->isDirty = 1;
}

/**
  @brief Set the Near Plane of the Camera
  
  @param camera
  @param near_plane
*/
void
camera_setNearPlane (Camera *camera, GLfloat near_plane)
{
  assert (camera != NULL);

  camera->projectSettings.near_plane = near_plane;
  camera->isDirty = 1;
}

/**
  @brief Set the Far Plane of the Camera

  @param camera
  @param far_plane
*/
void
camera_setFarPlane (Camera *camera, GLfloat far_plane)
{
  assert (camera != NULL);

  camera->projectSettings.far_plane = far_plane;
  camera->isDirty = 1;
}
