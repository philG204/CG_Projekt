#ifndef MESH_H
#define MESH_H
#define MAX 256
#include <GL/glew.h>

typedef struct Mesh{
    char* name;
    GLuint vbo;
    GLuint vao;
    int vertexCount;
} Mesh;

Mesh* mesh_init(char* objDir);
void mesh_draw(Mesh* mesh);

#endif // MESH_H
