#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

typedef struct LightSource
{
  GLfloat x;
  GLfloat y;
  GLfloat z;

  float ambient[4];
  float diffuse[4];
  float specular[4];
} LightSource;

typedef struct MaterialLight
{
  float emissive[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float shininess;
} MaterialLight;

// void gouraudLightning(const vec3 lightDir, const MaterialLight material);
/**
 *  @brief Setzt die Beleuchtung nach dem Gouraud-Shading-Verfahren um.
 *
 *  @param lightDir
 *  @param modelViewMatrix
 */
void gouraudLightning (const GLfloat *lightDir,
                       const GLfloat *modelViewMatrix);

#endif // LIGHT_H
