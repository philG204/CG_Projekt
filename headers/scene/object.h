#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../renderer/mesh.h"
#include "light.h"


typedef struct Material {
    GLuint* shader,
    GLuint** textures,
    GLfloat* rgb_values, 
    GLfloat* transparancy
    MaterialLight* light
} Material;

typedef struct Object {
    Mesh* mesh,
    Material* material,
    GLfloat* modelMatrix
} Object;

Object* object_init(char* vertObj, char** shader, char** textures, float* rgb_values = NULL, float* transparancy, float** light);
void object_transformation(Object* object, GLfloat* translation = NULL, GLfloat* scale = NULL, float rotaion = NULL);
void object_draw(Object* object);

#endif