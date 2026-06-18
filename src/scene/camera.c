
#include <GL/glew.h>
#include <stdio.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/camera.h"


Camera *
camera_init(CameraSettings *cameraSettings,
            ProjectionSettings *projectionSettings)
{
    Camera *camera = malloc(sizeof(Camera));
    if (camera == NULL)
    {
        return NULL;
    } 


    camera->viewProjMatrix = malloc(16 * sizeof(GLfloat));
    if (camera->viewProjMatrix == NULL)
    {
        free(camera);
        return NULL;
    }

    GLfloat viewMatrix[16];
    GLfloat projectionMatrix[16];

    lookAt(viewMatrix,
           cameraSettings->eye,
           cameraSettings->center,
           cameraSettings->up);

    perspective(projectionMatrix,
                projectionSettings->fovy,
                projectionSettings->aspect,
                projectionSettings->near_plane,
                projectionSettings->far_plane);

    multiplyMatracies(camera->viewProjMatrix,
                      projectionMatrix,
                      viewMatrix);

    camera->position = (Pos){0};
    camera->viewDirection = 0.0f;

    return camera;
}

void
camera_update (Camera *camera)
{
}
