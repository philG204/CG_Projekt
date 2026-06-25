#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/renderer/mesh.h"
#include "../../headers/renderer/shader.h"
#include "../../headers/renderer/texture.h"
#include "../../headers/scene/light.h"
#include "../../headers/scene/object.h"
#include "../../headers/utilities/config.h"


static void
activate_material_textures (Material *material)
{
  assert (material != NULL);

  GLuint shader = material->shaderObject->shader;

  if (material->baseTexture != NULL)
    {
      glActiveTexture (GL_TEXTURE0);
      glBindTexture (GL_TEXTURE_2D, material->baseTexture->textureId);

      glUniform1i (glGetUniformLocation (shader, "baseTexture"), 0);
    }

  for (int i = 0; i < material->overlayTextureCount; i++)
    {
      glActiveTexture (GL_TEXTURE1 + i);
      glBindTexture (GL_TEXTURE_2D,
                     material->overlayTextures[i]->textureId);

      char uniformName[64];

      snprintf (uniformName, sizeof (uniformName),
                "overlayTextures[%d]", i);

      glUniform1i (glGetUniformLocation (shader, uniformName), 1 + i);
    }

  glUniform1i (glGetUniformLocation (shader, "overlayTextureCount"),
               material->overlayTextureCount);
}

/**
 *  @brief

static void
activate_material_textures (Material *material)
{
  assert (material != NULL);

  GLuint shader = material->shaderObject->shader;

  if (material->baseTexture != NULL)
    {
      glActiveTexture (GL_TEXTURE0);
      glBindTexture (GL_TEXTURE_2D, material->baseTexture->textureId);

      glUniform1i (glGetUniformLocation (shader, "baseTexture"), 0);
    }

  for (int i = 0; i < material->overlayTextureCount; i++)
    {
      glActiveTexture (GL_TEXTURE1 + i);
      glBindTexture (GL_TEXTURE_2D,
                     material->overlayTextures[i]->textureId);

      char uniformName[64];

      snprintf (uniformName, sizeof (uniformName),
                "overlayTextures[%d]", i);

      glUniform1i (glGetUniformLocation (shader, uniformName), 1 + i);
    }

  glUniform1i (glGetUniformLocation (shader, "overlayTextureCount"),
               material->overlayTextureCount);
}

static void
object_load_config (Object *object, const char *configPath)
{
  assert (object != NULL);
  assert (configPath != NULL);

  char line[512];

  if (config_find_line_by_key (configPath, "name", line, sizeof (line)))
    {
      config_parse_string_value (line, object->name, sizeof (object->name));
    }

  if (config_find_line_by_key (configPath, "mesh", line, sizeof (line)))
    {
      config_parse_string_value (line, object->meshObject->meshName,
                                 sizeof (object->meshObject->meshName));
    }

  if (config_find_line_by_key (configPath, "shader", line, sizeof (line)))
    {
      config_parse_string_value (
          line, object->material->shaderObject->shaderName,
          sizeof (object->material->shaderObject->shaderName));
    }

  if (config_find_line_by_key (configPath, "isLight", line, sizeof (line)))
    {
      config_parse_int_value (line, &object->isLight);
    }

  if (config_find_line_by_key (configPath, "emissive", line, sizeof (line)))
    {
      config_parse_vec4_value (line, object->material->light->emissive);
    }

  if (config_find_line_by_key (configPath, "ambient", line, sizeof (line)))
    {
      config_parse_vec4_value (line, object->material->light->ambient);
    }

  if (config_find_line_by_key (configPath, "diffuse", line, sizeof (line)))
    {
      config_parse_vec4_value (line, object->material->light->diffuse);
    }

  if (config_find_line_by_key (configPath, "specular", line, sizeof (line)))
    {
      config_parse_vec4_value (line, object->material->light->specular);
    }

  if (config_find_line_by_key (configPath, "transparency", line,
                               sizeof (line)))
    {
      float transparencyValue = 0.0f;

      if (config_parse_float_value (line, &transparencyValue))
        {
          object->material->transparency = (int)transparencyValue;
        }
    }

  if (object->transformation != NULL)
    {
      if (config_find_line_by_key (configPath, "translation", line,
                                   sizeof (line)))
        {
          config_parse_vec3_value (line, object->transformation->translation);
        }

      if (config_find_line_by_key (configPath, "scaling", line, sizeof (line)))
        {
          config_parse_vec3_value (line, object->transformation->scaling);
        }

      if (config_find_line_by_key (configPath, "rotation", line,
                                   sizeof (line)))
        {
          config_parse_vec3_value (line, object->transformation->rotation);
        }

      if (config_find_line_by_key (configPath, "rotationCircle", line,
                                   sizeof (line)))
        {
          config_parse_vec3_value (line,
                                   object->transformation->rotaionCircle);
        }
    }
}

Object *
object_init (char *objDir)
{
  assert (objDir != NULL);

  Object *object = malloc (sizeof (Object));
  Material *material = malloc (sizeof (Material));
  MaterialLight *materialLight = malloc (sizeof (MaterialLight));
  Texture **overlayTextures = malloc (MAX_TEXTURES * sizeof (Texture *));
  Transformation *transformation = malloc (sizeof (Transformation));
  MeshObject *meshObject = malloc (sizeof (MeshObject));
  ShaderObject *shaderObject = malloc (sizeof (ShaderObject));

  if (object == NULL || material == NULL || materialLight == NULL
      || overlayTextures == NULL || transformation == NULL || meshObject == NULL
      || shaderObject == NULL)
    {
      printf ("object_init: malloc/calloc fehlgeschlagen\n");
      return NULL;
    }

  transformation->translation[0] = 0.0f;
  transformation->translation[1] = 0.0f;
  transformation->translation[2] = 0.0f;

  transformation->scaling[0] = 1.0f;
  transformation->scaling[1] = 1.0f;
  transformation->scaling[2] = 1.0f;

  transformation->rotation[0] = 0.0f;
  transformation->rotation[1] = 0.0f;
  transformation->rotation[2] = 0.0f;

  transformation->rotaionCircle[0] = 0.0f;
  transformation->rotaionCircle[1] = 0.0f;
  transformation->rotaionCircle[2] = 0.0f;

  materialLight->emissive[0] = 0.0f;
  materialLight->emissive[1] = 0.0f;
  materialLight->emissive[2] = 0.0f;
  materialLight->emissive[3] = 1.0f;

  materialLight->ambient[0] = 0.2f;
  materialLight->ambient[1] = 0.2f;
  materialLight->ambient[2] = 0.2f;
  materialLight->ambient[3] = 1.0f;

  materialLight->diffuse[0] = 0.8f;
  materialLight->diffuse[1] = 0.8f;
  materialLight->diffuse[2] = 0.8f;
  materialLight->diffuse[3] = 1.0f;

  materialLight->specular[0] = 1.0f;
  materialLight->specular[1] = 1.0f;
  materialLight->specular[2] = 1.0f;
  materialLight->specular[3] = 1.0f;

  materialLight->shininess = 32.0f;

  material->shaderObject = shaderObject;
  material->baseTexture = NULL;
  material->overlayTextures = overlayTextures;
  material->overlayTextureCount = 0;
  material->light = materialLight;

  object->material = material;
  object->transformation = transformation;
  object->meshObject = meshObject;
  object->modelMatrix = malloc (16 * sizeof (GLfloat));

  char configPath[512];

  snprintf (configPath, sizeof (configPath), "assets/%s/object.cfg", objDir);

  object_load_config (object, configPath);

  object->material->light = materialLight;

  object->modelMatrix = malloc (16 * sizeof (GLfloat));
  identity (object->modelMatrix);

  printf ("object_init loaded:\n");
  printf ("  name         = %s\n", object->name);
  printf ("  meshName     = %s\n", object->meshObject->meshName);
  printf ("  shaderName     = %s\n",
          object->material->shaderObject->shaderName);
  printf ("  textures     = %d\n", object->material->texture_count);
  printf ("  transparency = %lf\n", object->material->transparency);
  printf ("  emissive     = [%f, %f, %f, %f]\n",
          object->material->light->emissive[0],
          object->material->light->emissive[1],
          object->material->light->emissive[2],
          object->material->light->emissive[3]);
  printf ("  ambient      = [%f, %f, %f, %f]\n",
          object->material->light->ambient[0],
          object->material->light->ambient[1],
          object->material->light->ambient[2],
          object->material->light->ambient[3]);
  printf ("  diffuse      = [%f, %f, %f, %f]\n",
          object->material->light->diffuse[0],
          object->material->light->diffuse[1],
          object->material->light->diffuse[2],
          object->material->light->diffuse[3]);
  printf ("  specular     = [%f, %f, %f, %f]\n",
          object->material->light->specular[0],
          object->material->light->specular[1],
          object->material->light->specular[2],
          object->material->light->specular[3]);
  printf ("  translation  = %f\n", object->transformation->translation[0]);
  printf ("  scaling      = %f\n", object->transformation->scaling[0]);
  printf ("  rotation     = %f\n", object->transformation->rotation[0]);

  return object;
}

void
object_transformation (Object *object, GLfloat *translation, GLfloat *scaling,
                       GLfloat *rotation)
{
  assert (object != NULL);
  assert (translation != NULL);
  assert (scaling != NULL);
  assert (rotation != NULL);
  
  const GLfloat radian = (GLfloat)3.14 / 180.0f;

  translate (object->modelMatrix, object->modelMatrix, translation);
  scale (object->modelMatrix, object->modelMatrix, scaling);

  const GLfloat radiansX = rotation[0] * radian;
  const GLfloat radiansY = rotation[1] * radian;
  const GLfloat radiansZ = rotation[2] * radian;

  rotatex (object->modelMatrix, object->modelMatrix, radiansX);
  rotatey (object->modelMatrix, object->modelMatrix, radiansY);
  rotatez (object->modelMatrix, object->modelMatrix, radiansZ);
}

void
object_draw (Object *object, GLfloat *viewProj, GLfloat *viewMatrix,
             GLfloat *projMatrix, LightSource **lightSources, int lightCounts,
             GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ)
{
  assert (object != NULL);
  assert (viewProj != NULL);
  assert (viewMatrix != NULL);
  assert (projMatrix != NULL);
  assert (lightSources != NULL);

  use_shader (object->material->shaderObject->shader);

  glUniform4f (glGetUniformLocation (object->material->shaderObject->shader,
                                     "materialEmission"),
               object->material->light->emissive[0],
               object->material->light->emissive[1],
               object->material->light->emissive[2],
               object->material->light->emissive[3]);
  glUniform4f (glGetUniformLocation (object->material->shaderObject->shader,
                                     "materialAmbient"),
               object->material->light->ambient[0],
               object->material->light->ambient[1],
               object->material->light->ambient[2],
               object->material->light->ambient[3]);
  glUniform4f (glGetUniformLocation (object->material->shaderObject->shader,
                                     "materialDiffuse"),
               object->material->light->diffuse[0],
               object->material->light->diffuse[1],
               object->material->light->diffuse[2],
               object->material->light->diffuse[3]);
  glUniform4f (glGetUniformLocation (object->material->shaderObject->shader,
                                     "materialSpecular"),
               object->material->light->specular[0],
               object->material->light->specular[1],
               object->material->light->specular[2],
               object->material->light->specular[3]);
  glUniform1f (glGetUniformLocation (object->material->shaderObject->shader,
                                     "materialShininess"),
               object->material->light->shininess);

  char uniformName[32];
  for (int i = 0; i < lightCounts; i++)
    {
      snprintf (uniformName, sizeof (uniformName), "lights[%d].position", i);
      glUniform3f (glGetUniformLocation (
                       object->material->shaderObject->shader, uniformName),
                   lightSources[i]->x, lightSources[i]->y, lightSources[i]->z);

      snprintf (uniformName, sizeof (uniformName), "lights[%d].diffuse", i);
      glUniform4f (glGetUniformLocation (
                       object->material->shaderObject->shader, uniformName),
                   lightSources[i]->diffuse[0], lightSources[i]->diffuse[1],
                   lightSources[i]->diffuse[2], lightSources[i]->diffuse[3]);

      snprintf (uniformName, sizeof (uniformName), "lights[%d].specular", i);
      glUniform4f (glGetUniformLocation (
                       object->material->shaderObject->shader, uniformName),
                   lightSources[i]->specular[0], lightSources[i]->specular[1],
                   lightSources[i]->specular[2], lightSources[i]->specular[3]);
    }

  glUniform1i (glGetUniformLocation (object->material->shaderObject->shader,
                                     "lightCount"),
               lightCounts);

  glUniform3f (
      glGetUniformLocation (object->material->shaderObject->shader, "viewPos"),
      cameraX, cameraY, cameraZ);

  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shaderObject->shader, "view"), 1,
      GL_FALSE, viewMatrix);

  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shaderObject->shader, "proj"), 1,
      GL_FALSE, projMatrix);

  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shaderObject->shader, "model"),
      1, GL_FALSE, object->modelMatrix);

  glBindVertexArray (object->meshObject->mesh->vao);

  activate_material_textures (object->material);

  glDrawArrays (GL_TRIANGLES, 0,
              (GLsizei)object->meshObject->mesh->vertexCount);
}
