/**
 * @file scene.h
 * @brief Scene management and rendering orchestration
 *
 * This module handles the scene structure containing objects, meshes, shaders,
 * cameras, and lighting, as well as rendering the complete scene with
 * post-processing.
 */

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

/**
 * @struct Scene
 * @brief Represents a complete 3D scene with all renderable content
 *
 * @var Scene::objects
 *      Array of pointers to renderable objects
 * @var Scene::object_count
 *      Number of objects currently in the scene
 * @var Scene::meshes
 *      Array of pointers to cached mesh geometries
 * @var Scene::mesh_count
 *      Number of meshes currently in cache
 * @var Scene::shaderObjects
 *      Array of pointers to shader programs and their metadata
 * @var Scene::shader_count
 *      Number of shaders in the scene
 * @var Scene::camera
 *      Pointer to the active camera
 * @var Scene::lights
 *      Array of pointers to light sources
 * @var Scene::framebuffer
 *      Framebuffer Object for potentially off-screen rendering
 * @var Scene::texturebuffer
 *      Texture used as the framebuffer attachment
 * @var Scene::lightCount
 *      Number of light sources in the scene
 * @var Scene::name
 *      Name/identifier of the scene
 */
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
 * @brief Initialize the scene and set up its framebuffer
 *
 * Creates a scene by loading all objects, meshes, and shaders from the
 * specified directories. Sets up the camera and lighting, and prepares the
 * framebuffer for rendering.
 *
 * @param meshDir
 *        Directory path containing mesh files
 * @param shaderDir
 *        Directory path containing shader files
 * @param objectDir
 *        Directory path containing object configuration files
 * @param scene_name
 *        Name of the scene to load
 * @param cameraSettings
 *        Pointer to initial camera settings (view position, target, up vector)
 * @param projectionSettings
 *        Pointer to projection settings (FOV, aspect ratio, near/far planes)
 * @return Pointer to initialized Scene structure, or NULL if loading failed
 */
Scene *scene_init (const char *meshDir, const char *shaderDir,
                   const char *objectDir, const char *scene_name,
                   const CameraSettings *cameraSettings,
                   const ProjectionSettings *projectionSettings);

/**
 * @brief Update and render the current scene
 *
 * Updates all animated objects and renders the scene using the current camera,
 * applying all lighting and material properties.
 *
 * @param scene
 *        Pointer to the scene to render
 */
void scene_update (const Scene *scene);

#endif // SCENE_H
