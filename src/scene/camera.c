
#include <GL/glew.h>
#include <stdio.h>

#include "../../headers/scene/camera.h"
#include "../../headers/math/matrixTransformation.h"

Camera* camera_init(CameraSettings* cameraSettings, ProjectionSettings* projectionSettings){
    Camera* camera = malloc(sizeof(Camera));
    Pos position;
    GLfloat viewDirection;

    camera->viewProjMatrix = malloc(16 * sizeof(GLfloat));
    lookAt(camera->viewProjMatrix, cameraSettings->eye, cameraSettings->center, cameraSettings->up);
    perspective(camera->viewProjMatrix, projectionSettings->fovy, projectionSettings->aspect, projectionSettings->near_plane, projectionSettings->far_plane);

    camera->position = position;
    camera->viewDirection = viewDirection;

    return camera;
}

void camera_update(Camera* camera){

}