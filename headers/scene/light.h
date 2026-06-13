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

#endif // LIGHT_H
