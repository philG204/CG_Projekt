
#include <GL/glew.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/camera.h"

Camera *
camera_init (CameraSettings *cameraSettings,
             ProjectionSettings *projectionSettings)
{
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

void
camera_update (Camera *camera)
{
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

/*
  Set the entire Camera Settings at once
*/
void
camera_setCameraSettings (Camera *camera, CameraSettings *cameraSettings)
{
  if (memcmp (&camera->settings, cameraSettings, sizeof (CameraSettings)) != 0)
    {
      memcpy (&camera->settings, cameraSettings, sizeof (CameraSettings));
      camera->isDirty = 1;
    }
}

/*
  Set the entire Projections Settings at once
*/
void
camera_setProjectionSettings (Camera *camera,
                              ProjectionSettings *projectionSettings)
{
  if (memcmp (&camera->settings, projectionSettings,
              sizeof (ProjectionSettings))
      != 0)
    {
      memcpy (&camera->settings, projectionSettings,
              sizeof (ProjectionSettings));
      camera->isDirty = 1;
    }
}

/*
  Set the Eye of the Camera
*/
void
camera_setEye (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  camera->settings.eye[0] = x;
  camera->settings.eye[1] = y;
  camera->settings.eye[2] = z;
  camera->isDirty = 1;
}

/*
  Set the Center of the Camera
*/
void
camera_setCenter (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  camera->settings.center[0] = x;
  camera->settings.center[1] = y;
  camera->settings.center[2] = z;
  camera->isDirty = 1;
}

/*
  Set the Up of the Camera
*/
void
camera_setUp (Camera *camera, GLfloat x, GLfloat y, GLfloat z)
{
  camera->settings.up[0] = x;
  camera->settings.up[1] = y;
  camera->settings.up[2] = z;
  camera->isDirty = 1;
}

/*
  Set the Fovy of the Camera
*/
void
camera_setFovy (Camera *camera, GLfloat fovy)
{
  camera->projectSettings.fovy = fovy;
  camera->isDirty = 1;
}

/*
  Set the Aspect of the Camera
*/
void
camera_setAspect (Camera *camera, GLfloat aspect)
{
  camera->projectSettings.aspect = aspect;
  camera->isDirty = 1;
}

/*
  Set the Near Plane of the Camera
*/
void
camera_setNearPlane (Camera *camera, GLfloat near_plane)
{
  camera->projectSettings.near_plane = near_plane;
  camera->isDirty = 1;
}

/*
  Set the Far Plane of the Camera
*/
void
camera_setFarPlane (Camera *camera, GLfloat far_plane)
{
  camera->projectSettings.far_plane = far_plane;
  camera->isDirty = 1;
}
