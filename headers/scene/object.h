#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>

#include "../renderer/texture.h"
#include "light.h"

#define PATH_LENGTH 256
#define MAX_TEXTURES 10

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
  GLfloat rotaionCircle[3];
} Transformation;

typedef struct Material
{
  ShaderObject *shaderObject;
  Texture **textures;
  int texture_count;
  GLfloat rgb_values;
  GLfloat transparency;
  MaterialLight *light;
} Material;

typedef struct Mesh Mesh;

typedef struct Object
{
  char name[PATH_LENGTH];
  MeshObject *meshObject;
  Material *material;
  GLfloat *modelMatrix;
  GLfloat *normalMatrix;
  Transformation *transformation;
  int isLight;
} Object;

Object *object_init (char *objDir);
void object_transformation (Object *object, GLfloat *translation,
                            GLfloat *scaling, GLfloat *rotation);
void object_draw (Object *object, GLfloat *viewProj, GLfloat *viewMatrix,
                  GLfloat *projMatrix, LightSource **lightSources,
                  int lightCounts, GLfloat cameraX, GLfloat cameraY,
                  GLfloat cameraZ);

#endif // OBJECT_H
