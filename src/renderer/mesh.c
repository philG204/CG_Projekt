#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "../../headers/renderer/loadObj.h"
#include "../../headers/renderer/mesh.h"

static void 
getNameWithoutExtension(const char *path, char *out, size_t outSize)
{
    // letzten Slash suchen
    const char *filename = strrchr(path, '/');

    if (filename)
        filename++;   // Slash überspringen
    else
        filename = path;

    // letzte Dateiendung suchen
    const char *dot = strrchr(filename, '.');

    size_t len;
    if (dot)
        len = dot - filename;
    else
        len = strlen(filename);

    if (len >= outSize)
        len = outSize - 1;

    strncpy(out, filename, len);
    out[len] = '\0';
}
 

Mesh *
mesh_init (char *meshFile)
{
  GLuint vao;
  GLuint vbo;

  size_t vertexCount = 0;

  Mesh *mesh = malloc (sizeof (Mesh));
  mesh->name = malloc(strlen(meshFile) + 1);


  float *vertices = loadObj (meshFile, &vertexCount);
  printf ("loaded vertecies\n");
  if (vertices == NULL)
    {
      // printf(stderr, "Fehler beim Laden von OBJ: %s/mesh.obj\n", objDir);
      vertexCount = 0;
      vao = 0;
      vbo = 0;

      return NULL;
    }

  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);

  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);

  glBufferData (GL_ARRAY_BUFFER, (vertexCount) * 8 * sizeof (float), vertices,
                GL_STATIC_DRAW);

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float),
                         (void *)0);
  glEnableVertexAttribArray (0);

  glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float),
                         (void *)(3 * sizeof (float)));
  glEnableVertexAttribArray (1);

  glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float),
                         (void *)(5 * sizeof (float)));
  glEnableVertexAttribArray (2);

  glBindVertexArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  mesh->vbo = vbo;
  mesh->vao = vao;
  mesh->vertexCount = vertexCount;
  char name[256];
  getNameWithoutExtension(meshFile, name, sizeof(name));
  strcpy(mesh->name, name);
  
  free (vertices);

  printf ("OBJ geladen: %s, VAO=%u, VBO=%u, Vertices=%zu\n", meshFile, vao,
          vbo, vertexCount);

  return mesh;
}

void
mesh_draw (Mesh *mesh)
{
  glDrawArrays (GL_TRIANGLES, 0, (GLsizei)mesh->vertexCount);
}
