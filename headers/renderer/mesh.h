#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#define MAX 256

typedef struct Mesh
{
  char *name;
  GLuint vbo;
  GLuint vao;
  int vertexCount;
} Mesh;

Mesh *mesh_init (char *objDir);
void mesh_draw (Mesh *mesh);

#endif // MESH_H
