#ifndef MESH_H
#define MESH_H
#include <GL/glew.h>

typedef struct Mesh{
    GLuint* vbo,
    GLuint* vao,
    Material* material
} Mesh;

Mesh* mesh_init(Material* material);
void mesh_draw(Mesh* mesh);

#endif