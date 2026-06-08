#ifndef SCENE_H
#define SCENE_H
#define MAX_OBJECTS 100
#define MAX_MESHES 100
#include <GL/glew.h>

#include "camera.h"
#include "light.h"
#include "object.h"
#include "../renderer/mesh.h"


typedef struct Scene {
    Object** objects;
    int object_count;
    Mesh** meshes;
    int mesh_count;
    Camera* camera;
    LightDirection light;
    char* name;
} Scene;

Scene* scene_init(char* meshDir, int mesh_count, char* scene_name, CameraSettings* cameraSettings, ProjectionSettings* projectionSettings);
void scene_add_object(Scene* scene, char* objDir, char* mesh, float* materialLight, int transparancy);
void scene_update(Scene scene, float input);

#endif