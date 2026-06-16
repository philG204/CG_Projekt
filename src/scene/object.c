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

Object *
object_init (char *objDir, float light[], int transparency)
{
  Object *object = malloc (sizeof (Object));
  Material *material = malloc (sizeof (Material));
  MaterialLight *objectLight;
  Texture **textures = malloc(sizeof(Texture*) * MAX_TEXTURES);

  GLuint shaderProgram = shader_init (objDir);

  if(shaderProgram == NULL){
    printf("no shader program!!!\n");
  }

  material->shader = shaderProgram;
  material->textures = textures;
  material->texture_count = 0;
  // material->rgb_values = rgb_values;
  // material->transparency = transparency;
  material->light = NULL;

  object->material = material;

  char texturePath[512];
  snprintf (texturePath, sizeof (texturePath), "assets/%s/Textures/", objDir);
  DIR *dir = opendir (texturePath);

  if (dir == NULL)
    {
      printf ("Ordner konnte nicht geöffnet werden.\n");
      return NULL;
    }

  struct dirent *entry;

  while ((entry = readdir (dir)) != NULL)
    {
      if (strcmp (entry->d_name, ".") == 0
          || strcmp (entry->d_name, "..") == 0)
        {
          continue;
        }
      char completeTexturePath[512];
      snprintf (completeTexturePath, sizeof (completeTexturePath),
                "assets/%s/Textures/%s", objDir, entry->d_name);
      // printf("%s\n", completeTexturePath);
      object->material->textures[object->material->texture_count]
          = texture_init (completeTexturePath, object->material->shader,
                          entry->d_name);
      object->material->texture_count++;
      if(object->material->textures[object->material->texture_count] == NULL){
        printf("object_init: texture is NULL!!!\n");
      }
    }

  closedir (dir);

  MaterialLight *materialLight = malloc (sizeof (MaterialLight));
  materialLight->emissive = light[0];
  materialLight->ambient = light[1];
  materialLight->diffuse = light[2];
  materialLight->specular = light[3];

  object->material->light = materialLight;

  object->modelMatrix = malloc (16 * sizeof (GLfloat));
  identity (object->modelMatrix);

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

  rotatex(object->modelMatrix, object->modelMatrix, rotation[0]);
  rotatey(object->modelMatrix, object->modelMatrix, rotation[1]);
  rotatez(object->modelMatrix, object->modelMatrix, rotation[2]);
}

void
object_draw (Object *object, GLfloat *cameraMatrix)
{
  printf("entering object draw call\n");
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  static float angleView = 0.0f;
  angleView += 0.0001f;
  float radius = 5.0f;
  //printf("angleView: %f\n", angleView);

  float time = (float)glfwGetTime();

  if(object == NULL){
    printf("object in draw call is NULL !!!!\n");
  }
  use_shader (object->material->shader);
  printf("after using shader\n");

   GLfloat rotationYVec = angleView;
    rotatey(object->modelMatrix, object->modelMatrix, rotationYVec);

    GLfloat rotationZVec = angleView;
    rotatez(object->modelMatrix, object->modelMatrix, rotationZVec);

  glUniformMatrix4fv (
      glGetUniformLocation (object->material->shader, "viewProj"), 1, GL_FALSE,
      cameraMatrix);
  glUniformMatrix4fv (glGetUniformLocation (object->material->shader, "model"),
                      1, GL_FALSE, object->modelMatrix);
  printf("after setting model and camera matrecies\n");
  glBindVertexArray (object->mesh->vao);
  printf("after setting vao\n");
  printf("texture count: %d\n", object->material->texture_count);
  for (int i = 0; i < object->material->texture_count; i++)
    {
      activate_texture (object->material->textures,
                        object->material->texture_count);
                        printf("after activate textures\n");
    }
    printf("after activate all textures\n");
  glDrawArrays (GL_TRIANGLES, 0, (GLsizei)object->mesh->vertexCount);
  printf("Draw call finished for object %s\n", object->name);
}
