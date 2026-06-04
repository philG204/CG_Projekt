#ifndef CAMERA_H
#define CAMERA_H

typedef struct LightDirection{
    float x,
    float y,
    float z 
} LightDirection;

typedef struct MaterialLight {
    float emissive,
    float ambient,
    float diffuse,
    float specular
} MaterialLight;

#endif