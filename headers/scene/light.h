#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

typedef struct LightDirection
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
} LightDirection;

typedef struct MaterialLight
{
  GLfloat emissive;
  GLfloat ambient;
  GLfloat diffuse;
  GLfloat specular;
} MaterialLight;

// Setzt die Beleuchtung nach dem Gouraud-Shading um.
// void gouraudLightning(const vec3 lightDir, const MaterialLight material);
void gouraudLightning (const GLfloat *lightDir,
                       const GLfloat *modelViewMatrix);

#endif // LIGHT_H
