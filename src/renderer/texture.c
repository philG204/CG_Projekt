#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/renderer/texture.h"
#include "../../headers/stb_image.h"
#include "../../headers/utilities/config.h"

#define MAX_CACHED_TEXTURES 128

typedef struct TextureCacheEntry
{
  char path[512];
  Texture *texture;
} TextureCacheEntry;

static TextureCacheEntry textureCache[MAX_CACHED_TEXTURES];
static int textureCacheCount = 0;

/**
 * @brief Get or load a texture with caching
 *
 * Checks if a texture is already loaded in the cache. If found,
 * returns the cached version. Otherwise, loads the texture and adds it to the
 * cache.
 *
 * @param texturePath Path to the texture file
 * @param shader OpenGL shader program handle
 * @param textureName Name of the texture variable in the shader
 * @return Pointer to Texture structure, or NULL if loading failed
 */
static Texture *
texture_get_or_load (const char *texturePath, GLuint shader,
                     const char *textureName)
{
  assert (texturePath != NULL);
  assert (textureName != NULL);

  for (int i = 0; i < textureCacheCount; i++)
    {
      if (strcmp (textureCache[i].path, texturePath) == 0)
        {
          printf ("Texture aus Cache verwendet: %s\n", texturePath);
          return textureCache[i].texture;
        }
    }

  Texture *texture
      = texture_init ((char *)texturePath, shader, (char *)textureName);

  if (texture == NULL)
    {
      printf ("texture_get_or_load: konnte Texture nicht laden: %s\n",
              texturePath);
      return NULL;
    }

  if (textureCacheCount >= MAX_CACHED_TEXTURES)
    {
      printf ("texture_get_or_load: Texture-Cache voll. Texture wird ohne "
              "Cache benutzt: %s\n",
              texturePath);
      return texture;
    }

  strncpy (textureCache[textureCacheCount].path, texturePath,
           sizeof (textureCache[textureCacheCount].path) - 1);

  textureCache[textureCacheCount]
      .path[sizeof (textureCache[textureCacheCount].path) - 1] = '\0';

  textureCache[textureCacheCount].texture = texture;
  textureCacheCount++;

  printf ("Texture neu geladen und gecached: %s\n", texturePath);

  return texture;
}

Texture *
texture_init (char *filename, const GLuint shaderProgram,
              const char *shaderVariable)
{
  assert (filename != NULL);
  assert (shaderVariable != NULL);

  Texture *texture = malloc (sizeof (Texture));
  assert (texture != NULL);

  GLuint textureId;

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char *image = stbi_load (filename, &width, &height, &channels, 4);

  if (image == NULL)
    {
      printf ("texture_init: Fehler beim Laden der Textur %s: %s\n", filename,
              stbi_failure_reason ());
      free (texture);
      return NULL;
    }

  stbi_set_flip_vertically_on_load (1);

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

  texture->textureId = textureId;
  texture->shaderProgramId = shaderProgram;

  strncpy (texture->shaderVariable, shaderVariable,
           sizeof (texture->shaderVariable) - 1);

  texture->shaderVariable[sizeof (texture->shaderVariable) - 1] = '\0';

  printf ("Textur geladen: %s, ID=%u, Größe=%dx%d, Kanäle=%d\n", filename,
          textureId, width, height, channels);

  return texture;
}

void
activate_texture (Texture **textures, int texture_count)
{
  assert (textures != NULL);

  for (int i = 0; i < texture_count; i++)
    {
      const Texture *texture = textures[i];

      assert (texture != NULL);

      glActiveTexture (GL_TEXTURE0 + i);

      glBindTexture (GL_TEXTURE_2D, texture->textureId);

      GLint location = glGetUniformLocation (
          textures[i]->shaderProgramId,
          textures[i]->shaderVariable /*"baseTexture"*/);
      glUniform1i (location, i);
    }
}

Texture *
texture_init_base_from_config (const char *configPath, GLuint shader)
{
  assert (configPath != NULL);

  char textureLine[512];

  if (!config_find_line_by_key (configPath, "baseTexture", textureLine,
                                sizeof (textureLine)))
    {
      printf (
          "texture_init_base_from_config: keine baseTexture gefunden in %s\n",
          configPath);
      return NULL;
    }

  char textureName[CONFIG_MAX_STRING_LENGTH];

  if (!config_parse_string_value (textureLine, textureName,
                                  sizeof (textureName)))
    {
      printf ("texture_init_base_from_config: baseTexture konnte nicht "
              "gelesen werden in %s\n",
              configPath);
      return NULL;
    }

  char completeTexturePath[512];

  snprintf (completeTexturePath, sizeof (completeTexturePath),
            "assets/Textures/%s", textureName);

  Texture *texture
      = texture_get_or_load (completeTexturePath, shader, "baseTexture");

  if (texture == NULL)
    {
      printf ("texture_init_base_from_config: Textur konnte nicht geladen "
              "werden: %s\n",
              completeTexturePath);
      return NULL;
    }

  printf ("BaseTexture geladen: %s\n", completeTexturePath);

  return texture;
}

int
texture_init_overlays_from_config (const char *configPath, GLuint shader,
                                   Texture **textures, int maxTextures)
{
  assert (configPath != NULL);
  assert (textures != NULL);

  char textureLine[512];

  if (!config_find_line_by_key (configPath, "overlayTextures", textureLine,
                                sizeof (textureLine)))
    {
      printf ("texture_init_overlays_from_config: keine overlayTextures "
              "gefunden in %s\n",
              configPath);
      return 0;
    }

  char textureNames[CONFIG_MAX_LIST_ITEMS][CONFIG_MAX_STRING_LENGTH];

  if (maxTextures > CONFIG_MAX_LIST_ITEMS)
    {
      maxTextures = CONFIG_MAX_LIST_ITEMS;
    }

  const int textureCount = config_parse_string_list_value (
      textureLine, textureNames, maxTextures);

  int loadedCount = 0;

  for (int i = 0; i < textureCount; i++)
    {
      char completeTexturePath[512];

      snprintf (completeTexturePath, sizeof (completeTexturePath),
                "assets/Textures/%s", textureNames[i]);

      Texture *texture
          = texture_get_or_load (completeTexturePath, shader, textureNames[i]);

      if (texture == NULL)
        {
          printf ("texture_init_overlays_from_config: Overlay konnte nicht "
                  "geladen werden: %s\n",
                  completeTexturePath);
          continue;
        }

      textures[loadedCount] = texture;
      loadedCount++;

      printf ("OverlayTexture geladen: %s\n", completeTexturePath);
    }

  return loadedCount;
}
