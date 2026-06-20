#ifndef LIGHT_H
#define LIGHT_H

typedef struct LightDirection{
    float x;
    float y;
    float z;
} LightDirection;

typedef struct MaterialLight {
    float emissive[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess;
} MaterialLight;

// Setzt die Beleuchtung nach dem Gouraud-Shading um.
//void gouraudLightning(const vec3 lightDir, const MaterialLight material);
void gouraudLightning(const GLfloat *lightDir, const GLfloat *modelViewMatrix);

#endif // LIGHT_H