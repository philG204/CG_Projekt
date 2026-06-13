#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "light.h"
#include "../renderer/mesh.h"
#include "../renderer/texture.h"


typedef struct Material {
    GLuint shader;
    Texture** textures;
    int texture_count;
    GLfloat rgb_values;
    GLfloat transparancy;
    MaterialLight* light;
} Material;

typedef struct Object {
    char* name;
    Mesh* mesh;
    Material* material;
    GLfloat* modelMatrix;
} Object;

Object* object_init(char* objDir, float light[], int transparancy);
void object_transformation(Object* object, GLfloat* translation, GLfloat* scale, float rotaion);
void object_draw(Object* object, GLfloat* cameraMatrix);

#endif // OBJECT_H
