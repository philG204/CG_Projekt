#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>

#include "../renderer/texture.h"
#include "light.h"

#define PATH_LENGTH 256

typedef struct Transformation
{
  GLfloat translation[3];
  GLfloat scaling[3];
  GLfloat rotation[3];
} Transformation;

typedef struct Material
{
  GLuint shader;
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
  char meshName[PATH_LENGTH];
  Mesh *mesh;
  Material *material;
  GLfloat *modelMatrix;
  Transformation *transformation;
} Object;

Object *object_init (char *objDir);
void object_transformation (Object *object, GLfloat *translation,
                            GLfloat *scaling, GLfloat *rotation);
void object_draw (Object *object, GLfloat *cameraMatrix);

#endif // OBJECT_H
