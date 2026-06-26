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

/**
 *  @brief
 *
 *  @param meshDir
 *  @param shaderDir
 *  @param scene_name
 *  @param cameraSettings
 *  @param projectionSettings
 *  @return
 */
Scene *scene_init (const char *meshDir, const char *shaderDir,
                   const char *objectDir, const char *scene_name,
                   const CameraSettings *cameraSettings,
                   const ProjectionSettings *projectionSettings);

/**
 *  @brief
 *
 *  @param scene
 */
void scene_update (const Scene *scene);

#endif // SCENE_H
