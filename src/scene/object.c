#include <GL/glew.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/renderer/mesh.h"
#include "../../headers/renderer/shader.h"
#include "../../headers/renderer/texture.h"
#include "../../headers/scene/light.h"
#include "../../headers/scene/object.h"
#include "../../headers/utilities/config.h"

static void
object_load_config(Object *object, const char *configPath)
{
    if (object == NULL || configPath == NULL || object->material == NULL) {
        return;
    }

    char line[512];

    if (config_find_line_by_key(configPath, "name", line, sizeof(line))) {
        config_parse_string_value(line,
                                  object->name,
                                  sizeof(object->name));
    }

    if (config_find_line_by_key(configPath, "mesh", line, sizeof(line))) {
        config_parse_string_value(line,
                                  object->meshName,
                                  sizeof(object->meshName));
    }

    if (object->isLight != NULL) {
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
    }

    if (config_find_line_by_key(configPath, "transparency", line, sizeof(line))) {
        float transparencyValue = 0.0f;

        if (config_parse_float_value(line, &transparencyValue)) {
            object->material->transparency = (int)transparencyValue;
        }
    }

    if (object->transformation != NULL) {
        if (config_find_line_by_key(configPath, "translation", line, sizeof(line))) {
            config_parse_vec3_value(line,
                                     object->transformation->translation);
        }

        if (config_find_line_by_key(configPath, "scaling", line, sizeof(line))) {
            config_parse_vec3_value(line,
                                     object->transformation->scaling);
        }

        if (config_find_line_by_key(configPath, "rotaion", line, sizeof(line))) {
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

  Object *object = malloc (sizeof (Object));
  Material *material = malloc (sizeof (Material));
  MaterialLight *materialLight = malloc (sizeof (MaterialLight));;
  Texture **textures = malloc(sizeof(Texture*) * MAX_TEXTURES);
  Transformation *transformation = malloc(sizeof(Transformation*));


  GLuint shaderProgram = shader_init (objDir);
 
  if(shaderProgram == 0){
    printf("object_init: no shader program for %s\n", objDir);
  }

  material->shader = shaderProgram;
  material->textures = textures;
  material->texture_count = 0;
  material->light = materialLight;

  object->material = material;
  object->transformation = transformation;

  char configPath[512];

  snprintf(configPath,
            sizeof(configPath),
            "assets/%s/object.cfg",
            objDir);

  object_load_config(object, configPath);
 
  material->texture_count = texture_init_from_config(objDir,
                                                       material->shader,
                                                       material->textures,
                                                       MAX_TEXTURES);

  //materialLight->emissive = 0.0f;
  //materialLight->ambient  = 0.2f;
  //materialLight->diffuse  = 0.8f;
  //materialLight->specular = 1.0f;

  object->material->light = materialLight;

  object->modelMatrix = malloc (16 * sizeof (GLfloat));
  identity (object->modelMatrix);

  
  printf("object_init loaded:\n");
    printf("  name         = %s\n", object->name);
    printf("  meshName     = %s\n", object->meshName);
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
  if(!(translation == NULL)){
    translate(object->modelMatrix, object->modelMatrix, translation);
  }

  if(!(scaling == NULL)){
    scale(object->modelMatrix, object->modelMatrix, scaling);
  }

  if(!(object->transformation->rotaionCircle == NULL)){
    int xCircle = object->transformation->rotaionCircle[0];
    rotatex(object->modelMatrix, object->modelMatrix, rotation[0] + xCircle);
  }
  
  if(!(object->transformation->rotaionCircle == NULL)){
    int yCircle = object->transformation->rotaionCircle[1];
    rotatex(object->modelMatrix, object->modelMatrix, rotation[1] + yCircle);
  }


  if(!(object->transformation->rotaionCircle == NULL)){
    int zCircle = object->transformation->rotaionCircle[2];
    rotatex(object->modelMatrix, object->modelMatrix, rotation[2] + zCircle);
  }
}

void
object_draw (Object *object, GLfloat *cameraMatrix, LightDirection **lightDirections, int lightCounts)
{
  use_shader (object->material->shader);

  glUniform4f(glGetUniformLocation(object->material->shader, "materialEmission"), object->material->light->emissive[0], object->material->light->emissive[1], object->material->light->emissive[2], object->material->light->emissive[3]);
  glUniform4f(glGetUniformLocation(object->material->shader, "materialAmbient"), object->material->light->ambient[0], object->material->light->ambient[1], object->material->light->ambient[2], object->material->light->ambient[3]);
  glUniform4f(glGetUniformLocation(object->material->shader, "materialDiffuse"), object->material->light->diffuse[0], object->material->light->diffuse[1], object->material->light->diffuse[2],object->material->light->diffuse[3]);
  glUniform4f(glGetUniformLocation(object->material->shader, "materialSpecular"), object->material->light->specular[0], object->material->light->specular[1], object->material->light->specular[2], object->material->light->specular[3]);
  glUniform1f(glGetUniformLocation(object->material->shader, "materialShininess"), object->material->light->shininess);  

  //if(lightDirections == NULL){
  //  printf("light is NULL!!!\n");
  //}
  for(int i=0;i<lightCounts;i++){
    glUniform3f(glGetUniformLocation(object->material->shader, "lightPos" + i), lightDirections[i]->x, lightDirections[i]->y, lightDirections[i]->z);
  }
  
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
