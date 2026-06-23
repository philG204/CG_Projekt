#include <assert.h>
#include <dirent.h>
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

    if (config_find_line_by_key(configPath, "mesh", line, sizeof(line))) {
        config_parse_string_value(line,
                                  object->meshObject->meshName,
                                  sizeof(object->meshObject->meshName));
    }

    if (config_find_line_by_key(configPath, "shader", line, sizeof(line))) {
        config_parse_string_value(line,
                                  object->material->shaderObject->shaderName,
                                  sizeof(object->material->shaderObject->shaderName));
    }

  
    if (config_find_line_by_key(configPath, "isLight", line, sizeof(line))) {
        config_parse_int_value(line, &object->isLight);
    }

    if (config_find_line_by_key(configPath, "emissive", line, sizeof(line))) {
        config_parse_float_value(line,
                                     object->material->light->emissive);
        printf("emissive: %f\n", object->material->light->emissive);
    }

    if (config_find_line_by_key(configPath, "ambient", line, sizeof(line))) {
        config_parse_vec4_value(line,
                                object->material->light->ambient);
    }

    if (config_find_line_by_key(configPath, "diffuse", line, sizeof(line))) {
        config_parse_vec4_value(line,
                                object->material->light->diffuse);
    }

    if (config_find_line_by_key(configPath, "specular", line, sizeof(line))) {
            config_parse_vec4_value(line,
                                object->material->light->specular);
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

        if (config_find_line_by_key(configPath, "rotation", line, sizeof(line))) {
            config_parse_vec3_value(line,
                                     object->transformation->rotation);
        }

        if (config_find_line_by_key(configPath, "rotationCircle", line, sizeof(line))) {
            config_parse_vec3_value(line,
                                     object->transformation->rotaionCircle);
        }
    }
}

Object *
object_init (char *objDir)
{
 assert(objDir != NULL);

 Object *object = calloc(1, sizeof(Object));
 Material *material = calloc(1, sizeof(Material));
 MaterialLight *materialLight = calloc(1, sizeof(MaterialLight));
 Texture **textures = calloc(MAX_TEXTURES, sizeof(Texture*));  
 Transformation *transformation = calloc(1, sizeof(Transformation));
 MeshObject *meshObject = calloc(1, sizeof(MeshObject));
 ShaderObject *shaderObject = calloc(1, sizeof(ShaderObject));

 if (object == NULL ||
    material == NULL ||
    materialLight == NULL ||
    textures == NULL ||
    transformation == NULL ||
    meshObject == NULL ||
    shaderObject == NULL)
  {
    printf("object_init: malloc/calloc fehlgeschlagen\n");
    return NULL;
  }

  transformation->scaling[0] = 1.0f;
  transformation->scaling[1] = 1.0f;
  transformation->scaling[2] = 1.0f;

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
  material->textures = textures;
  material->texture_count = 0;
  material->light = materialLight;

  object->material = material;
  object->transformation = transformation;
  object->meshObject = meshObject;

  char configPath[512];

  snprintf (configPath, sizeof (configPath), "assets/%s/object.cfg", objDir);

  object_load_config (object, configPath);

  object_load_config(object, configPath);
 
  material->texture_count = texture_init_from_config(objDir,
                                                       material->shaderObject->shader,
                                                       material->textures,
                                                       MAX_TEXTURES);

  object->material->light = materialLight;

  object->modelMatrix = malloc (16 * sizeof (GLfloat));
  identity (object->modelMatrix);

  object->normalMatrix = malloc (16 * sizeof (GLfloat));
  inverse(object->normalMatrix, object->modelMatrix);
  
  
  printf("object_init loaded:\n");
    printf("  name         = %s\n", object->name);
    printf("  meshName     = %s\n", object->meshObject->meshName);
    printf("  shaderName     = %s\n", object->material->shaderObject->shaderName);
    printf("  textures     = %d\n", object->material->texture_count);
    printf("  transparency = %d\n", object->material->transparency);
    printf("  emissive     = %f\n", object->material->light->emissive);
    printf("  ambient      = %f\n", object->material->light->ambient);
    printf("  diffuse      = %f\n", object->material->light->diffuse);
    printf("  specular     = %f\n", object->material->light->specular);
    printf("  translation  = %f\n", object->transformation->translation[0]);
    printf("  scaling      = %f\n", object->transformation->scaling[0]);
    printf("  rotation     = %f\n", object->transformation->rotation[0]);

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

  translate (object->modelMatrix, object->modelMatrix, translation);
  scale (object->modelMatrix, object->modelMatrix, scaling);

  if(object->transformation->rotaionCircle != NULL){
    rotation[0] += object->transformation->rotaionCircle[0];
    rotation[1] += object->transformation->rotaionCircle[1];
    rotation[2] += object->transformation->rotaionCircle[2];
  }

  rotatex(object->modelMatrix, object->modelMatrix, rotation[0]);
  rotatey(object->modelMatrix, object->modelMatrix, rotation[1]);
  rotatez(object->modelMatrix, object->modelMatrix, rotation[2]);
  //printf("rotaion x: %f, rotaion y: %f, rotation z: %f\n", rotation[0], rotation[1], rotation[2]);
}

void
object_draw (Object *object, GLfloat *viewProj, GLfloat *viewMatrix, GLfloat* projMatrix, LightDirection **lightDirections, int lightCounts)
{
  assert (object != NULL);
  assert (viewProj != NULL);
  assert (viewMatrix != NULL);
  assert (projMatrix != NULL);
  assert (lightDirections != NULL);

  use_shader (object->material->shaderObject->shader);

  glUniform4f(glGetUniformLocation(object->material->shaderObject->shader, "materialEmission"), object->material->light->emissive[0], object->material->light->emissive[1], object->material->light->emissive[2], object->material->light->emissive[3]);
  glUniform4f(glGetUniformLocation(object->material->shaderObject->shader, "materialAmbient"), object->material->light->ambient[0], object->material->light->ambient[1], object->material->light->ambient[2], object->material->light->ambient[3]);
  glUniform4f(glGetUniformLocation(object->material->shaderObject->shader, "materialDiffuse"), object->material->light->diffuse[0], object->material->light->diffuse[1], object->material->light->diffuse[2],object->material->light->diffuse[3]);
  glUniform4f(glGetUniformLocation(object->material->shaderObject->shader, "materialSpecular"), object->material->light->specular[0], object->material->light->specular[1], object->material->light->specular[2], object->material->light->specular[3]);
  glUniform1f(glGetUniformLocation(object->material->shaderObject->shader, "materialShininess"), object->material->light->shininess);  

  for(int i=0;i<lightCounts;i++){
    glUniform3f(glGetUniformLocation(object->material->shaderObject->shader, "lightPos" + i), lightDirections[i]->x, lightDirections[i]->y, lightDirections[i]->z);
  }
  
  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shaderObject->shader, "view"), 1, GL_FALSE,
      viewMatrix);
  
  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shaderObject->shader, "proj"), 1, GL_FALSE,
      projMatrix);

  glUniformMatrix4fv (glGetUniformLocation (object->material->shaderObject->shader, "model"),
                      1, GL_FALSE, object->modelMatrix);
 
  glUniformMatrix4fv(glGetUniformLocation (object->material->shaderObject->shader, "normalM"),
                      1, GL_FALSE, object->modelMatrix);

  glBindVertexArray (object->meshObject->mesh->vao);

  for (int i = 0; i < object->material->texture_count; i++)
    {
      activate_texture (object->material->textures,
                        object->material->texture_count);
    }

  glDrawArrays (GL_TRIANGLES, 0, (GLsizei)object->meshObject->mesh->vertexCount);
}
