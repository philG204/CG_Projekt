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

/**
 *  @brief
 *
 *  @param meshFile
 */
Mesh *mesh_init (char *meshFile);

/**
 *  @brief
 *
 *  @param
 */
void mesh_draw (const Mesh *mesh);

#endif // MESH_H
