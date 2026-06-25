#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

typedef struct Texture
{
  GLuint textureId;
  GLuint shaderProgramId;
  char shaderVariable[256];
} Texture;


Texture *texture_init (char *filename);

void activate_texture (Texture **textures, int texture_count);

int texture_init_from_config (const char *configPath, GLuint shader,
                              Texture **textures, int maxTextures);

Texture *texture_init_base_from_config (const char *objDir, GLuint shader);

int texture_init_overlays_from_config (const char *objDir, GLuint shader,
                                       Texture **textures, int maxTextures);

#endif // TEXTURE_H
