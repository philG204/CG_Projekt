/**
 * @file mesh.h
 * @brief Mesh data structure and operations for 3D geometry
 *
 * This module provides functionality for loading and rendering mesh geometry,
 * including vertex buffer management and drawing operations.
 */

#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#define MAX 256

/**
 * @struct Mesh
 * @brief Represents a 3D mesh geometry with vertex buffer objects
 *
 * @var Mesh::name
 *      Name/identifier of the mesh
 * @var Mesh::vbo
 *      Vertex Buffer Object handle containing vertex data
 * @var Mesh::vao
 *      Vertex Array Object handle for describing vertex attributes
 * @var Mesh::vertexCount
 *      Number of vertices in the mesh
 */
typedef struct Mesh
{
  char *name;
  GLuint vbo;
  GLuint vao;
  int vertexCount;
} Mesh;

/**
 * @brief Initialize a mesh from an OBJ file
 *
 * Loads mesh geometry from an OBJ file and creates vertex buffer objects
 * for GPU rendering.
 *
 * @param meshFile Path to the .obj mesh file
 * @return Pointer to initialized Mesh structure, or NULL if loading failed
 */
Mesh *mesh_init (char *meshFile);

/**
 * @brief Draw a mesh to the current framebuffer
 *
 * Renders the mesh geometry using the currently bound shader program
 * and any vertex attributes that have been configured.
 *
 * @param mesh Pointer to the mesh to draw
 */
void mesh_draw (const Mesh *mesh);

#endif // MESH_H
