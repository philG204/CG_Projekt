#include <GL/glew.h>
#include <stdio.h>

#include "../../headers/renderer/texture.h"
#include "../../headers/stb_image.h"

Texture *
texture_init (char *filename, GLuint shaderProgram, char *shaderVariable)
{
  Texture *texture = malloc (sizeof (Texture));
  GLuint textureId;

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char *image = stbi_load (filename, &width, &height, &channels, 4);
  // printf("loaded file %s image %s\n", filename, image);
  if (image == NULL)
    {
      // printf(stderr, "Fehler beim Laden der Textur %s: %s\n", filename,
      // stbi_failure_reason());
      return NULL;
    }

  glGenTextures (1, &textureId);
  glBindTexture (GL_TEXTURE_2D, textureId);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, image);

  glGenerateMipmap (GL_TEXTURE_2D);

  stbi_image_free (image);

  glBindTexture (GL_TEXTURE_2D, 0);

  printf ("Textur geladen: %s, ID=%u, Größe=%dx%d, Kanäle=%d\n", filename,
          textureId, width, height, channels);

  texture->textureId = textureId;
  texture->shaderProgramId = shaderProgram;
  texture->shaderVariable = shaderVariable;

  return texture;
}

void
activate_texture (Texture *textures[], int texture_count)
{
  for (int i = 0; i < texture_count; i++)
    {
      glActiveTexture (GL_TEXTURE0 + i);
      glBindTexture (GL_TEXTURE_2D, textures[i]->textureId);

      GLint location = glGetUniformLocation (textures[i]->shaderProgramId,
                                             textures[i]->shaderVariable);
      glUniform1i (location, i);
    }
}
