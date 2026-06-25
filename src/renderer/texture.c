#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/renderer/texture.h"
#include "../../headers/stb_image.h"
#include "../../headers/utilities/config.h"
#include "../../headers/utilities/fileOperations.h"

#define MAX_CACHED_TEXTURES 128

typedef struct TextureCacheEntry
{
  char path[512];
  Texture *texture;
} TextureCacheEntry;

static TextureCacheEntry textureCache[MAX_CACHED_TEXTURES];
static int textureCacheCount = 0;

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
      = texture_init ((char *)texturePath);

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

int
texture_init_from_config (const char *configPath, GLuint shader,
                          Texture **textures, int maxTextures)
{
  assert (configPath != NULL);
  assert (textures != NULL);

  char textureLine[512];
  char completeConfigPath[512];
  snprintf (completeConfigPath, sizeof (completeConfigPath),
            "assets/%s/object.cfg", configPath);
  printf ("open file: %s\n", completeConfigPath);

  printf ("texture_init_from_config: %s\n", completeConfigPath);
  if (!config_find_line_by_key (completeConfigPath, "textures", textureLine,
                                sizeof (textureLine)))
    {
      printf (
          "texture_init_from_config: keine textures-Zeile gefunden in %s\n",
          completeConfigPath);
      return 0;
    }

  char textureNames[CONFIG_MAX_LIST_ITEMS][CONFIG_MAX_STRING_LENGTH];

  if (maxTextures > CONFIG_MAX_LIST_ITEMS)
    {
      maxTextures = CONFIG_MAX_LIST_ITEMS;
    }

  int textureCount = config_parse_string_list_value (textureLine, textureNames,
                                                     maxTextures);

  int loadedCount = 0;

  for (int i = 0; i < textureCount; i++)
    {
      char completeTexturePath[512];
      printf ("texture index: %d\n", i);
      snprintf (completeTexturePath, sizeof (completeTexturePath),
                "assets/Textures/%s", textureNames[i]);

      Texture *texture
          = texture_get_or_load (completeTexturePath, shader, textureNames[i]);
      printf ("texture loaded: %s\n", completeTexturePath);
      if (texture == NULL)
        {
          printf ("texture_init_from_config: texture konnte nicht geladen "
                  "werden: %s\n",
                  completeTexturePath);
          continue;
        }

      textures[loadedCount] = texture;
      loadedCount++;
    }

  return loadedCount;
}

Texture *
texture_init (char *filename)
{
  assert (filename != NULL);

  Texture *texture = calloc (1, sizeof (Texture));

  if (texture == NULL)
    {
      printf ("texture_init: calloc fehlgeschlagen\n");
      return NULL;
    }

  GLuint textureId;

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char *image = stbi_load (filename, &width, &height, &channels, 4);

  if (image == NULL)
    {
      printf ("texture_init: Fehler beim Laden der Textur %s: %s\n",
              filename, stbi_failure_reason ());
      free (texture);
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

  texture->textureId = textureId;

  printf ("Textur geladen: %s, ID=%u, Größe=%dx%d, Kanäle=%d\n",
          filename, textureId, width, height, channels);

  return texture;
}

void
activate_texture (Texture **textures, int texture_count)
{
  assert (textures != NULL);

  for (int i = 0; i < texture_count; i++)
    {
      Texture *texture = textures[i];

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
texture_init_base_from_config (const char *objDir, GLuint shader)
{
  assert (objDir != NULL);

  char textureLine[512];
  char completeConfigPath[512];

  snprintf (completeConfigPath, sizeof (completeConfigPath),
            "assets/%s/object.cfg", objDir);

  if (!config_find_line_by_key (completeConfigPath, "baseTexture",
                                textureLine, sizeof (textureLine)))
    {
      printf ("texture_init_base_from_config: keine baseTexture gefunden in %s\n",
              completeConfigPath);
      return NULL;
    }

  char textureName[CONFIG_MAX_STRING_LENGTH];

  if (!config_parse_string_value (textureLine, textureName,
                                  sizeof (textureName)))
    {
      printf ("texture_init_base_from_config: baseTexture konnte nicht gelesen werden in %s\n",
              completeConfigPath);
      return NULL;
    }

  char completeTexturePath[512];

  snprintf (completeTexturePath, sizeof (completeTexturePath),
            "assets/Textures/%s", textureName);

  Texture *texture =
      texture_get_or_load (completeTexturePath, shader, "baseTexture");

  if (texture == NULL)
    {
      printf ("texture_init_base_from_config: Textur konnte nicht geladen werden: %s\n",
              completeTexturePath);
      return NULL;
    }

  printf ("BaseTexture geladen: %s\n", completeTexturePath);

  return texture;
}

int
texture_init_overlays_from_config (const char *objDir, GLuint shader,
                                   Texture **textures, int maxTextures)
{
  assert (objDir != NULL);
  assert (textures != NULL);

  char textureLine[512];
  char completeConfigPath[512];

  snprintf (completeConfigPath, sizeof (completeConfigPath),
            "assets/%s/object.cfg", objDir);

  if (!config_find_line_by_key (completeConfigPath, "overlayTextures",
                                textureLine, sizeof (textureLine)))
    {
      printf ("texture_init_overlays_from_config: keine overlayTextures gefunden in %s\n",
              completeConfigPath);
      return 0;
    }

  char textureNames[CONFIG_MAX_LIST_ITEMS][CONFIG_MAX_STRING_LENGTH];

  if (maxTextures > CONFIG_MAX_LIST_ITEMS)
    {
      maxTextures = CONFIG_MAX_LIST_ITEMS;
    }

  int textureCount = config_parse_string_list_value (textureLine, textureNames,
                                                     maxTextures);

  int loadedCount = 0;

  for (int i = 0; i < textureCount; i++)
    {
      char completeTexturePath[512];

      snprintf (completeTexturePath, sizeof (completeTexturePath),
                "assets/Textures/%s", textureNames[i]);

      Texture *texture =
          texture_get_or_load (completeTexturePath, shader, textureNames[i]);

      if (texture == NULL)
        {
          printf ("texture_init_overlays_from_config: Overlay konnte nicht geladen werden: %s\n",
                  completeTexturePath);
          continue;
        }

      textures[loadedCount] = texture;
      loadedCount++;

      printf ("OverlayTexture geladen: %s\n", completeTexturePath);
    }

  return loadedCount;
}


