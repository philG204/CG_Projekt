#ifndef SCENE_H
#define SCENE_H
#include <GL/glew.h>

#include "object.h"
#include "camera.h"
#include "light.h"


typedef struct Scene {
    Object** objects,
    Camera camera,
    Light light

} Scene;

Scene* scene_init(char** objectNames);
void scene_update(Scene scene, float input);

#endif