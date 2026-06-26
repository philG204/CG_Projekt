/**
 * @file camera.c
 * @brief Camera initialization and management
 *
 * This file implements camera creation with view and projection matrices,
 * camera updates, and settings modifications.
 */

#include <assert.h>
#include <complex.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/camera.h"

/**
 * @brief Initialize a camera with view and projection matrices
 *
 * Creates a camera with the specified view parameters (eye position, target
 * center, up vector) and projection parameters (FOV, aspect ratio, near/far
 * planes).
 *
 * @param cameraSettings Pointer to CameraSettings structure with eye, center,
 * and up vectors
 * @param projectionSettings Pointer to ProjectionSettings structure with FOV,
 * aspect, and clipping planes
 * @return Pointer to initialized Camera structure
 */
Camera *
camera_init (const CameraSettings *cameraSettings,
             const ProjectionSettings *projectionSettings)
{
  assert (cameraSettings != NULL);
  assert (projectionSettings != NULL);

  Camera *camera = malloc (sizeof (Camera));
  const Pos position = { .x = cameraSettings->eye[0],
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

void
camera_update (Camera *camera)
{
  assert (camera != NULL);

  // nothing has changed, so no recalculation needed
  if (camera->isDirty == 0)
    {
      return;
    }

  // Recalculate the Matrices
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

void
camera_setCameraSettings (Camera *camera, const CameraSettings *cameraSettings)
{
  assert (camera != NULL);
  assert (cameraSettings != NULL);

  if (memcmp (&camera->settings, cameraSettings, sizeof (CameraSettings)) != 0)
    {
      memcpy (&camera->settings, cameraSettings, sizeof (CameraSettings));
      camera->isDirty = 1;
    }
}

void
camera_setProjectionSettings (Camera *camera,
                              const ProjectionSettings *projectionSettings)
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

void
camera_setEye (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  assert (camera != NULL);

  camera->settings.eye[0] = x;
  camera->settings.eye[1] = y;
  camera->settings.eye[2] = z;
  camera->isDirty = 1;
}

void
camera_setCenter (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  assert (camera != NULL);

  camera->settings.center[0] = x;
  camera->settings.center[1] = y;
  camera->settings.center[2] = z;
  camera->isDirty = 1;
}

void
camera_setUp (Camera *camera, const GLfloat x, const GLfloat y,
              const GLfloat z)
{
  assert (camera != NULL);

  camera->settings.up[0] = x;
  camera->settings.up[1] = y;
  camera->settings.up[2] = z;
  camera->isDirty = 1;
}

void
camera_setFovy (Camera *camera, const GLfloat fovy)
{
  assert (camera != NULL);

  camera->projectSettings.fovy = fovy;
  camera->isDirty = 1;
}

void
camera_setAspect (Camera *camera, const GLfloat aspect)
{
  assert (camera != NULL);

  camera->projectSettings.aspect = aspect;
  camera->isDirty = 1;
}

void
camera_setNearPlane (Camera *camera, const GLfloat near_plane)
{
  assert (camera != NULL);

  camera->projectSettings.near_plane = near_plane;
  camera->isDirty = 1;
}

void
camera_setFarPlane (Camera *camera, const GLfloat far_plane)
{
  assert (camera != NULL);

  camera->projectSettings.far_plane = far_plane;
  camera->isDirty = 1;
}
