#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>

#include "../renderer/mesh.h"
#include "../renderer/texture.h"
#include "light.h"

#define PATH_LENGTH 256
#define MAX_TEXTURES 10
#define MAX_OVERLAY_TEXTURES 8

typedef struct MeshObject
{
  char meshName[PATH_LENGTH];
  Mesh *mesh;
} MeshObject;

typedef struct ShaderObject
{
  char shaderName[PATH_LENGTH];
  GLuint shader;
} ShaderObject;

typedef struct Transformation
{
  GLfloat translation[3];
  GLfloat scaling[3];
  GLfloat rotation[3];
  GLfloat rotationCircle[3];
} Transformation;

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
 *  @brief
 *
 *  @param configPath
 *  @return
 */
Object *object_init (const char *configPath);

/**
 *  @brief Translatiert, rotiert und skaliert ein Objekt.
 *
 *  @param object
 *  @param translation
 *  @param scaling
 *  @param rotation
 */
void object_transformation (const Object *object, const GLfloat *translation,
                            const GLfloat *scaling, const GLfloat *rotation);

/**
 *  @brief
 *
 *  @param object
 *  @param viewProj
 *  @param viewMatrix
 *  @param projMatrix
 *  @param lightSources
 *  @param lightCounts
 *  @param cameraX
 *  @param cameraY
 *  @param cameraZ
 */
void object_draw (const Object *object, const GLfloat *viewProj,
                  const GLfloat *viewMatrix, const GLfloat *projMatrix,
                  LightSource **lightSources, int lightCounts, GLfloat cameraX,
                  GLfloat cameraY, GLfloat cameraZ);

#endif // OBJECT_H
