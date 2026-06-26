/**
 * @file object.h
 * @brief Scene objects with mesh, material, and transformation properties
 *
 * This module defines the Object structure and functions for creating,
 * transforming, and rendering 3D objects in the scene with materials and
 * lighting.
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>

#include "../renderer/mesh.h"
#include "../renderer/texture.h"
#include "light.h"

#define PATH_LENGTH 256
#define MAX_TEXTURES 10
#define MAX_OVERLAY_TEXTURES 8

/**
 * @struct MeshObject
 * @brief Container for a mesh and its associated name
 *
 * @var MeshObject::meshName
 *      Name/path identifier of the mesh
 * @var MeshObject::mesh
 *      Pointer to the Mesh data structure
 */
typedef struct MeshObject
{
  char meshName[PATH_LENGTH];
  Mesh *mesh;
} MeshObject;

/**
 * @struct ShaderObject
 * @brief Container for a shader program and its associated name
 *
 * @var ShaderObject::shaderName
 *      Name/identifier of the shader
 * @var ShaderObject::shader
 *      OpenGL shader program handle
 */
typedef struct ShaderObject
{
  char shaderName[PATH_LENGTH];
  GLuint shader;
} ShaderObject;

/**
 * @struct Transformation
 * @brief Apply translation, rotation, and scaling transformations
 *
 * @var Transformation::translation
 *      3D translation vector [x, y, z]
 * @var Transformation::scaling
 *      3D scaling factors [x, y, z]
 * @var Transformation::rotation
 *      Rotation angles in degrees [x, y, z] (Euler angles)
 * @var Transformation::rotationCircle
 *      Rotation axis for circular/orbital motion [x, y, z]
 */
typedef struct Transformation
{
  GLfloat translation[3];
  GLfloat scaling[3];
  GLfloat rotation[3];
  GLfloat rotationCircle[3];
} Transformation;

/**
 * @struct Material
 * @brief Material properties and textures for rendering
 *
 * @var Material::shaderObject
 *      Pointer to the shader program used for rendering
 * @var Material::baseTexture
 *      Primary/base texture of the material
 * @var Material::overlayTextures
 *      Array of additional overlay textures for layering
 * @var Material::overlayTextureCount
 *      Number of overlay textures in use
 * @var Material::rgb_values
 *      RGB color multiplier for the material
 * @var Material::light
 *      Pointer to material lighting properties
 */
typedef struct Material
{
  ShaderObject *shaderObject;
  Texture *baseTexture;
  Texture **overlayTextures;
  int overlayTextureCount;
  GLfloat rgb_values;
  MaterialLight *light;
} Material;

typedef Mesh Mesh;

/**
 * @struct Object
 * @brief Represents a 3D object in the scene
 *
 * @var Object::name
 *      Unique name/identifier of the object
 * @var Object::meshObject
 *      Pointer to the mesh geometry
 * @var Object::material
 *      Pointer to the material and texture properties
 * @var Object::modelMatrix
 *      4x4 model transformation matrix for positioning
 * @var Object::normalMatrix
 *      3x3 normal matrix for lighting calculations
 * @var Object::transformation
 *      Transformation parameters (translation, rotation, scale)
 * @var Object::isTransparent
 *      Flag indicating if object uses transparency (1=transparent, 0=opaque)
 * @var Object::isLight
 *      Flag indicating if object is a light source (1=light, 0=regular object)
 */
typedef struct Object
{
  char name[PATH_LENGTH];
  MeshObject *meshObject;
  Material *material;
  GLfloat *modelMatrix;
  GLfloat *normalMatrix;
  Transformation *transformation;
  int isTransparent;
  int isLight;
} Object;

/**
 * @brief Initialize an object from a configuration file
 *
 * Loads mesh, shader, and material properties from a configuration file
 * and creates an Object with all required data.
 *
 * @param configPath
 *        Path to the object configuration file
 * @return Pointer to initialized Object structure, or NULL if loading failed
 */
Object *object_init (const char *configPath);

/**
 * @brief Apply transformation (translation, rotation, scaling) to an object
 *
 * Translates, rotates, and scales the object by computing and updating
 * the model matrix based on the transformation parameters.
 *
 * @param object
 *        Pointer to the object to transform
 * @param translation
 *        3D translation vector [x, y, z]
 * @param scaling
 *        3D scaling factors [x, y, z]
 * @param rotation
 *        Rotation angles in degrees [x, y, z] (Euler angles)
 */
void object_transformation (const Object *object, const GLfloat *translation,
                            const GLfloat *scaling, const GLfloat *rotation);

/**
 * @brief Draw an object to the current framebuffer
 *
 * Renders the object using its shader and material with the provided
 * view and projection matrices, considering all light sources in the scene.
 *
 * @param object
 *        Pointer to the object to draw
 * @param viewProj
 *        Combined view-projection matrix
 * @param viewMatrix
 *        View matrix for transforming to camera space
 * @param projMatrix
 *        Projection matrix for perspective transformation
 * @param lightSources
 *        Array of pointers to light sources affecting the object
 * @param lightCounts
 *        Number of light sources
 * @param cameraX
 *        Camera position X coordinate
 * @param cameraY
 *        Camera position Y coordinate
 * @param cameraZ
 *        Camera position Z coordinate
 */
void object_draw (const Object *object, const GLfloat *viewProj,
                  const GLfloat *viewMatrix, const GLfloat *projMatrix,
                  LightSource **lightSources, int lightCounts, GLfloat cameraX,
                  GLfloat cameraY, GLfloat cameraZ);

#endif // OBJECT_H
