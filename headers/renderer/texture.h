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
 *  @brief Init a texture
 *
 *  @param filename The path of the texture
 *  @param shaderProgram The programm handle
 *  @param shaderVariable The name of the shader variable
 */
Texture *texture_init (char *filename, GLuint shaderProgram,
                       const char *shaderVariable);

/**
 *  @brief Activate a Texture
 *
 *  @param textures Out parameter for textures
 *  @param texture_count Number of textures
 */
void activate_texture (Texture **textures, int texture_count);

/**
 * @brief Initialize base texture from config
 *
 * @param objDir
 * @param shader The programm handle
 * @return Texture
 */
Texture *texture_init_base_from_config (const char *objDir, GLuint shader);

/**
 * @brief Initialize overlay textures from config
 *
 * @param objDir
 * @param shader The programm handle
 * @param textures Out parameter for textures
 * @param maxTextures Number of textures
 * @return int
 */
int texture_init_overlays_from_config (const char *objDir, GLuint shader,
                                       Texture **textures, int maxTextures);
#endif // TEXTURE_H
