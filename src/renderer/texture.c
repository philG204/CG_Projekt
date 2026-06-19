#include <GL/glew.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int
read_next_data_line (FILE *file, char *buffer, size_t bufferSize)
{
  while (fgets (buffer, bufferSize, file) != NULL)
    {
      char *line = config_trim (buffer);

      if (line[0] == '\0')
        {
          continue;
        }

      if (line[0] == '/' && line[1] == '/')
        {
          continue;
        }

      memmove (buffer, line, strlen (line) + 1);
      return 1;
    }

  return 0;
}

static Texture *
texture_get_or_load (const char *texturePath, GLuint shader,
                     const char *textureName)
{
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

int
texture_init_from_config (const char *configPath, GLuint shader,
                          Texture **textures, int maxTextures)
{
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

      snprintf (completeTexturePath, sizeof (completeTexturePath),
                "assets/Textures/%s", textureNames[i]);

      Texture *texture
          = texture_get_or_load (completeTexturePath, shader, textureNames[i]);

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
texture_init (char *filename, GLuint shaderProgram, char *shaderVariable)
{
  printf ("entering texture_init with filename: %s\n", filename);
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
  if (textures == NULL)
    {
      printf ("textures are NULL!!!\n");
    }
  for (int i = 0; i < texture_count; i++)
    {
      if (textures[i] == NULL)
        {
          printf ("texture is NULL!!!\n");
        }

      glActiveTexture (GL_TEXTURE0 + i);

      glBindTexture (GL_TEXTURE_2D, textures[i]->textureId);

      GLint location = glGetUniformLocation (textures[i]->shaderProgramId,
                                             textures[i]->shaderVariable);
      glUniform1i (location, i);
    }
}
