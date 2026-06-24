#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/renderer/shader.h"
#include "../../headers/scene/loadObjectList.h"
#include "../../headers/scene/scene.h"
#include "../../headers/utilities/fileOperations.h"

static void
loadShaderDirectoryStructure (Scene *scene, char *shaderDir)
{
  if (scene == NULL || shaderDir == NULL)
    {
      printf ("loadShaderDirectoryStructure: scene oder shaderDir ist NULL\n");
      return;
    }

  char shaderPath[512];

  snprintf (shaderPath, sizeof (shaderPath), "assets/%s", shaderDir);

  printf ("Shader-Hauptordner: %s\n", shaderPath);

  DIR *dirShader = opendir (shaderPath);

  if (dirShader == NULL)
    {
      printf ("Shader-Hauptordner konnte nicht geöffnet werden: %s\n",
              shaderPath);
      return;
    }

  struct dirent *entryShader;

  while ((entryShader = readdir (dirShader)) != NULL)
    {
      printf ("1 while loop %s\n", entryShader->d_name);

      if (strcmp (entryShader->d_name, ".") == 0
          || strcmp (entryShader->d_name, "..") == 0)
        {
          continue;
        }

      char shaderSubFolderPath[512];

      snprintf (shaderSubFolderPath, sizeof (shaderSubFolderPath),
                "assets/%s/%s", shaderDir, entryShader->d_name);

      struct stat subFolderStat;

      if (stat (shaderSubFolderPath, &subFolderStat) != 0)
        {
          printf ("Pfad konnte nicht geprüft werden: %s\n",
                  shaderSubFolderPath);
          continue;
        }

      if (!S_ISDIR (subFolderStat.st_mode))
        {
          continue;
        }

      printf ("Shader-Unterordner gefunden: %s\n", shaderSubFolderPath);

      if (scene->shader_count >= MAX_SHADER_COUNT)
        {
          printf ("MAX_SHADER_COUNT erreicht\n");
          break;
        }

      ShaderObject *shaderObject = malloc (sizeof (ShaderObject));

      if (shaderObject == NULL)
        {
          printf ("ShaderObject konnte nicht alloziert werden.\n");
          continue;
        }

      memset (shaderObject, 0, sizeof (ShaderObject));

      strncpy (shaderObject->shaderName, entryShader->d_name,
               sizeof (shaderObject->shaderName) - 1);

      shaderObject->shaderName[sizeof (shaderObject->shaderName) - 1] = '\0';

      printf ("before shader_init\n");

      /*
       * Wichtig:
       * Hier wird der ORDNER übergeben,
       * nicht fragmentShader.vs oder vertexShader.vs.
       */
      shaderObject->shader = shader_init (shaderSubFolderPath);

      printf ("after shader_init\n");

      if (shaderObject->shader == 0)
        {
          printf ("Shader konnte nicht geladen werden: %s\n",
                  shaderSubFolderPath);

          free (shaderObject);
          continue;
        }

      scene->shaderObjects[scene->shader_count] = shaderObject;

      printf ("Shader geladen: name='%s', id=%u\n",
              scene->shaderObjects[scene->shader_count]->shaderName,
              scene->shaderObjects[scene->shader_count]->shader);

      scene->shader_count++;
    }

  closedir (dirShader);
}

Scene *
scene_init (char *meshDir, char *shaderDir,
            char *scene_name, CameraSettings *cameraSettings,
            ProjectionSettings *projectionSettings)
{
  assert (shaderDir != NULL);
  assert (meshDir != NULL);
  assert (scene_name != NULL);
  assert (projectionSettings != NULL);
  assert (cameraSettings != NULL);

  printf ("entering scene_init\n");
  Scene *scene = malloc (sizeof (Scene));
  Camera *camera;
  LightSource **lights
      = malloc (sizeof (LightSource *) * MAX_LIGHT_OBJECTS);
  Mesh **meshes = malloc (sizeof (Mesh *) * MAX_MESHES);
  ShaderObject **shaderObjects
      = malloc (sizeof (ShaderObject *) * MAX_SHADER_COUNT);
  Object **objects = malloc (sizeof (Object *) * MAX_OBJECTS);

  scene->shaderObjects = shaderObjects;
  scene->mesh_count = 0;
  scene->shader_count = 0;
  scene->object_count = 0;
  scene->objects = objects;
  scene->camera = malloc (sizeof (Camera));
  scene->lights = lights;
  scene->lightCount = 0;

  camera = camera_init (cameraSettings, projectionSettings);
  scene->camera = camera;

  printf ("LOADING SHADERS!!!!\n");
  loadShaderDirectoryStructure (scene, shaderDir);
  printf ("DONE WITH LOADING SHADERS!!!!\n");
  printf ("SHADER COUNT %d\n", scene->shader_count);

  char meshPath[512];
  snprintf (meshPath, sizeof (meshPath), "assets/%s", meshDir);
  printf ("%s\n", meshPath);
  DIR *dirMesh = opendir (meshPath);

  if (dirMesh == NULL)
    {
      printf ("Ordner konnte nicht geöffnet werden.\n");
      return NULL;
    }

  struct dirent *entryMesh;

  while ((entryMesh = readdir (dirMesh)) != NULL)
    {
      if (strcmp (entryMesh->d_name, ".") == 0
          || strcmp (entryMesh->d_name, "..") == 0)
        {
          continue;
        }
      char completeMeshPath[512];
      snprintf (completeMeshPath, sizeof (completeMeshPath), "assets/%s/%s",
                meshDir, entryMesh->d_name);
      printf ("%s\n", completeMeshPath);

      printf ("before mesh_init\n");
      meshes[scene->mesh_count] = mesh_init (completeMeshPath);
      printf ("after mesh_init\n");
      scene->mesh_count++;
    }

  scene->meshes = meshes;
  closedir (dirMesh);

  return scene;
}

void
scene_add_object (Scene *scene, char *objDir)
{
  assert (scene != NULL);
  assert (objDir != NULL);

  printf ("entering scene_add_object\n");
  Object *object = object_init (objDir);

  if (object == NULL)
    {
      printf ("scene_add_object: object is NULL\n");
      return;
    }

  if (scene->object_count >= MAX_OBJECTS)
    {
      printf ("scene_add_object: MAX_OBJECTS erreicht\n");
      return;
    }

  if (object->isLight == 1)
    {
      LightSource *lightObject = malloc (sizeof (LightSource));
      lightObject->x = object->transformation->translation[0];
      lightObject->y = object->transformation->translation[1];
      lightObject->z = object->transformation->translation[2];

      lightObject->diffuse[0] = object->material->light->diffuse[0];
      lightObject->diffuse[1] = object->material->light->diffuse[1];
      lightObject->diffuse[2] = object->material->light->diffuse[2];
      lightObject->diffuse[3] = object->material->light->diffuse[3];

      lightObject->specular[0] = object->material->light->specular[0];
      lightObject->specular[1] = object->material->light->specular[1];
      lightObject->specular[2] = object->material->light->specular[2];
      lightObject->specular[3] = object->material->light->specular[3];
      scene->lights[scene->lightCount] = lightObject;
      scene->lightCount++;
    }

  if (object->meshObject->meshName[0] == '\0')
    {
      printf (
          "scene_add_object: object %s hat keinen meshName aus der Config\n",
          object->name);
      return;
    }

  char meshNameWithoutExtension[256];

  getNameWithoutExtension (object->meshObject->meshName,
                           meshNameWithoutExtension,
                           sizeof (meshNameWithoutExtension));

  printf ("scene_add_object: object = %s\n", object->name);
  printf ("scene_add_object: mesh from config = %s\n",
          object->meshObject->meshName);
  printf ("scene_add_object: mesh without extension = %s\n",
          meshNameWithoutExtension);

  object->meshObject->mesh = NULL;

  for (int i = 0; i < scene->mesh_count; i++)
    {
      if (scene->meshes[i] == NULL)
        {
          continue;
        }

      printf ("scene_add_object: compare '%s' with '%s'\n",
              scene->meshes[i]->name, meshNameWithoutExtension);

      if (strcmp (scene->meshes[i]->name, meshNameWithoutExtension) == 0)
        {
          object->meshObject->mesh = scene->meshes[i];

          printf ("scene_add_object: mesh %s assigned to object %s\n",
                  scene->meshes[i]->name, object->name);

          break;
        }
    }

  if (object->meshObject->mesh == NULL)
    {
      printf ("scene_add_object: kein passendes Mesh gefunden für object %s. "
              "Gesucht: %s\n",
              object->name, meshNameWithoutExtension);
      return;
    }

  object->material->shaderObject->shader = 0;

  for (int i = 0; i < scene->shader_count; i++)
    {
      if (scene->shaderObjects[i] == NULL)
        {
          continue;
        }

      if (strcmp (scene->shaderObjects[i]->shaderName,
                  object->material->shaderObject->shaderName)
          == 0)
        {
          object->material->shaderObject->shader
              = scene->shaderObjects[i]->shader;

          printf ("scene_add_object: shader %s assigned to object %s\n",
                  scene->shaderObjects[i]->shaderName, object->name);

          break;
        }
    }

  if (object->material->shaderObject->shader == 0)
    {
      printf ("scene_add_object: kein passendes Shader gefunden für object "
              "%s. Gesucht: %s\n",
              object->name, meshNameWithoutExtension);
      return;
    }

  scene->objects[scene->object_count] = object;
  scene->object_count++;
  printf ("shader program id: %d\n", object->material->shaderObject->shader);
}

void
scene_update (Scene *scene)
{
  assert (scene != NULL);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Object *object = NULL;

  for (int j = 0; j < scene->object_count; j++)
    {
      object = scene->objects[j];
      if (object == NULL)
        {
          continue;
        }

      if (object->meshObject->mesh == NULL)
        {
          printf ("mesh from object %d in scene is NULL!!!\n", j);
          continue;
        }

      identity (object->modelMatrix);
      
      object_transformation (object,
                             object->transformation->translation,
                             object->transformation->scaling,
                             object->transformation->rotation);
      object_draw (object, scene->camera->viewProj,
                   scene->camera->view, scene->camera->projection,
                   scene->lights, scene->lightCount, scene->camera->position.x, scene->camera->position.y, scene->camera->position.z);
    }
}
