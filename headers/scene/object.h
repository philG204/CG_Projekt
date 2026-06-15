#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "light.h"
#include "../renderer/mesh.h"
#include "../renderer/texture.h"
#include "../../headers/math/matrixTransformation.h"


typedef struct Material {
    GLuint shader;
    Texture** textures;
    int texture_count;
    GLfloat rgb_values;
    GLfloat transparency;
    MaterialLight* light;
} Material;

typedef struct Object {
    char* name;
    Mesh* mesh;
    Material* material;
    GLfloat* modelMatrix;
} Object;

Object* object_init(char* objDir, float light[], int transparency);
void object_transformation(Object* object, GLfloat* translation, GLfloat* scaling, float rotation);
void object_draw(Object* object, GLfloat* cameraMatrix);

#endif // OBJECT_H
