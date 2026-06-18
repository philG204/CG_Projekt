#ifndef OBJECT_H
#define OBJECT_H

#define PATH_LENGTH 256

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "light.h"
#include "../renderer/mesh.h"
#include "../renderer/texture.h"
#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/loadObjectList.h"

typedef struct Transformation{
    float translation[3];
    float scaling[3];
    float rotation[3];
} Transformation;

typedef struct Material {
    GLuint shader;
    Texture** textures;
    int texture_count;
    GLfloat rgb_values;
    GLfloat transparency;
    MaterialLight* light;
} Material;

typedef struct Object {
    char name[PATH_LENGTH];
    char meshName[PATH_LENGTH];
    Mesh* mesh;
    Material* material;
    GLfloat* modelMatrix;
    Transformation* transformation;
} Object;

Object* object_init(char* objDir);
void object_transformation(Object* object, GLfloat* translation, GLfloat* scaling, GLfloat* rotation);
void object_draw(Object* object, GLfloat* cameraMatrix);

#endif // OBJECT_H
