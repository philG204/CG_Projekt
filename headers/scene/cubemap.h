#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <GL/glew.h>
#include "../../headers/scene/scene.h"


GLuint *cubemap_init (Scene *scene, char *textureFileNames[], char *objectName);
void cubemap_draw (Object *object, GLfloat *cameraMatrix, GLuint *texture);

#endif // CUBEMAP_H