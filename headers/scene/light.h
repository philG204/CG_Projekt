#ifndef LIGHT_H
#define LIGHT_H

typedef struct LightDirection{
    float x;
    float y;
    float z;
} LightDirection;

typedef struct MaterialLight {
    float emissive;
    float ambient;
    float diffuse;
    float specular;
} MaterialLight;

// Setzt die Beleuchtung nach dem Gouraud-Shading um.
//void gouraudLightning(const vec3 lightDir, const MaterialLight material);
void gouraudLightning(const GLfloat *lightDir, const GLfloat *modelViewMatrix);

#endif // LIGHT_H