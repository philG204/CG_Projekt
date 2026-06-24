#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>

#include "../renderer/mesh.h"
#include "camera.h"
#include "light.h"
#include "object.h"

#define MAX_OBJECTS 100
#define MAX_LIGHT_OBJECTS 100
#define MAX_MESHES 100
#define PATH_LENGTH 256
#define MAX_SHADER_COUNT 100

typedef struct Scene
{
  Object **objects;
  int object_count;
  Mesh **meshes;
  int mesh_count;
  ShaderObject **shaderObjects;
  int shader_count;
  Camera *camera;
  LightSource **lights;
  GLuint framebuffer;
  GLuint texturebuffer;
  int lightCount;
  char name[PATH_LENGTH];
} Scene;

Scene *scene_init (char *meshDir, char *shaderDir, 
                   char *scene_name,
                   CameraSettings *cameraSettings,
                   ProjectionSettings *projectionSettings);
void scene_add_object (Scene *scene, char *object);
void scene_update (Scene *scene);

#endif // SCENE_H
