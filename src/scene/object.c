#include <assert.h>
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/renderer/mesh.h"
#include "../../headers/renderer/shader.h"
#include "../../headers/renderer/texture.h"
#include "../../headers/scene/light.h"
#include "../../headers/scene/object.h"
#include "../../headers/utilities/config.h"

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
      config_parse_string_value (line, object->meshName,
                                 sizeof (object->meshName));
    }

  if (object->material->light != NULL)
    {
      if (config_find_line_by_key (configPath, "emissive", line,
                                   sizeof (line)))
        {
          config_parse_float_value (line, &object->material->light->emissive);
          printf ("emissive: %f\n", object->material->light->emissive);
        }

      if (config_find_line_by_key (configPath, "ambient", line, sizeof (line)))
        {
          config_parse_float_value (line, &object->material->light->ambient);
        }

      if (config_find_line_by_key (configPath, "diffuse", line, sizeof (line)))
        {
          config_parse_float_value (line, &object->material->light->diffuse);
        }

      if (config_find_line_by_key (configPath, "specular", line,
                                   sizeof (line)))
        {
          config_parse_float_value (line, &object->material->light->specular);
        }
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

      if (config_find_line_by_key (configPath, "rotation", line, sizeof (line)))
        {
          config_parse_vec3_value (line, object->transformation->rotation);
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

  Texture **textures = malloc (sizeof (Texture *) * MAX_TEXTURES);
  Transformation *transformation = malloc (sizeof (Transformation));

  transformation->translation[0] = 0.0f;
  transformation->translation[1] = 0.0f;
  transformation->translation[2] = 0.0f;
  transformation->scaling[0] = 1.0f;
  transformation->scaling[1] = 1.0f;
  transformation->scaling[2] = 1.0f;
  transformation->rotation[0] = 0.0f;
  transformation->rotation[1] = 0.0f;
  transformation->rotation[2] = 0.0f;

  GLuint shaderProgram = shader_init (objDir);

  if (shaderProgram == 0)
    {
      printf ("object_init: no shader program for %s\n", objDir);
    }

  material->shader = shaderProgram;
  material->textures = textures;
  material->texture_count = 0;
  material->light = materialLight;

  object->material = material;
  object->transformation = transformation;

  char configPath[512];

  snprintf (configPath, sizeof (configPath), "assets/%s/object.cfg", objDir);

  object_load_config (object, configPath);

  material->texture_count = texture_init_from_config (
      objDir, material->shader, material->textures, MAX_TEXTURES);

  // materialLight->emissive = 0.0f;
  // materialLight->ambient  = 0.2f;
  // materialLight->diffuse  = 0.8f;
  // materialLight->specular = 1.0f;

  object->material->light = materialLight;

  object->modelMatrix = malloc (16 * sizeof (GLfloat));
  identity (object->modelMatrix);

  printf ("object_init loaded:\n");
  printf ("  name         = %s\n", object->name);
  printf ("  meshName     = %s\n", object->meshName);
  printf ("  textures     = %d\n", object->material->texture_count);
  printf ("  transparency = %f\n", object->material->transparency);
  printf ("  emissive     = %f\n", object->material->light->emissive);
  printf ("  ambient      = %f\n", object->material->light->ambient);
  printf ("  diffuse      = %f\n", object->material->light->diffuse);
  printf ("  specular     = %f\n", object->material->light->specular);
  printf ("  translation  = %f\n", object->transformation->translation[0]);
  printf ("  scaling      = %f\n", object->transformation->scaling[0]);
  printf ("  rotation     = %f\n", object->transformation->rotation[0]);

  return object;
}

// Translatiert, rotiert und skaliert ein Objekt.
void
object_transformation (Object *object, GLfloat *translation, GLfloat *scaling,
                       GLfloat *rotation)
{
  assert (object != NULL);
  assert (translation != NULL);
  assert (scaling != NULL);
  assert (rotation != NULL);

  const GLfloat radian = (GLfloat)M_PI / 180.0f;

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
object_draw (Object *object, GLfloat *cameraMatrix)
{
  assert (object != NULL);
  assert (cameraMatrix != NULL);

  use_shader (object->material->shader);

  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shader, "viewProj"), 1, GL_FALSE,
      cameraMatrix);
  glUniformMatrix4fv (glGetUniformLocation (object->material->shader, "model"),
                      1, GL_FALSE, object->modelMatrix);

  glBindVertexArray (object->mesh->vao);

  for (int i = 0; i < object->material->texture_count; i++)
    {
      activate_texture (object->material->textures,
                        object->material->texture_count);
    }

  glDrawArrays (GL_TRIANGLES, 0, (GLsizei)object->mesh->vertexCount);
}
