#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

typedef struct Texture
{
  GLuint textureId;
  GLuint shaderProgramId;
  char shaderVariable[256];
} Texture;

/**
 *  @brief
 *
 *  @param configPath
 *  @param shader
 *  @param textures
 *  @param maxTextures
 */
int texture_init_from_config (const char *configPath, GLuint shader,
                              Texture **textures, int maxTextures);

/**
 *  @brief
 *
 *  @param filename
 *  @param shaderProgram
 *  @param shaderVariable
 */
Texture *texture_init (char *filename, GLuint shaderProgram,
                       char *shaderVariable);

/**
 *  @brief
 *
 *  @param textures
 *  @param texture_count
 */
void activate_texture (Texture **textures, int texture_count);

/**
 * @brief
 *
 * @param objDir
 * @return Texture
 */
Texture *texture_init_base_from_config (const char *objDir, GLuint shader);

/**
 * @brief
 *
 * @param objDir
 * @param shader
 * @param textures
 * @param maxTextures
 * @return int
 */
int texture_init_overlays_from_config (const char *objDir, GLuint shader,
                                       Texture **textures, int maxTextures);
#endif // TEXTURE_H
