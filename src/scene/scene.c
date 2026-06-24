#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "../../headers/math/matrixTransformation.h"
#include "../../headers/scene/loadObjectList.h"
#include "../../headers/scene/scene.h"
#include "../../headers/utilities/fileOperations.h"

Scene *
scene_init (char *meshDir, int mesh_count, char *scene_name,
            CameraSettings *cameraSettings,
            ProjectionSettings *projectionSettings)
{
  assert (meshDir != NULL);
  assert (scene_name != NULL);
  assert (cameraSettings != NULL);
  assert (projectionSettings != NULL);

  printf ("entering scene_init\n");
  Scene *scene = malloc (sizeof (Scene));
  Camera *camera;
  LightDirection light;
  Mesh **meshes = malloc (sizeof (Mesh *) * MAX_MESHES);
  Object **objects = malloc (sizeof (Object *) * MAX_OBJECTS);
  scene->mesh_count = mesh_count;
  scene->object_count = 0;
  scene->objects = objects;
  scene->camera = malloc (sizeof (Camera));
  strncpy (scene->name, scene_name, sizeof (scene->name));

  light.x = 0.0f;
  light.y = 4.0f;
  light.z = 0.0f;
  scene->light = light;

  camera = camera_init (cameraSettings, projectionSettings);
  scene->camera = camera;

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

  glGenFramebuffers (1, &scene->framebuffer);
  glBindFramebuffer (GL_FRAMEBUFFER, scene->framebuffer);

  glGenTextures (1, &scene->texturebuffer);
  glBindTexture (GL_TEXTURE_2D, scene->texturebuffer);

  // TODO add proper size handling
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB,
                GL_UNSIGNED_BYTE, NULL);

  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                          scene->texturebuffer, 0);

  if (glCheckFramebufferStatus (GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    perror ("framebuffer incomplete!");

  glBindFramebuffer (GL_FRAMEBUFFER, 0);

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

  if (object->meshName[0] == '\0')
    {
      printf (
          "scene_add_object: object %s hat keinen meshName aus der Config\n",
          object->name);
      return;
    }

  char meshNameWithoutExtension[256];

  getNameWithoutExtension (object->meshName, meshNameWithoutExtension,
                           sizeof (meshNameWithoutExtension));

  printf ("scene_add_object: object = %s\n", object->name);
  printf ("scene_add_object: mesh from config = %s\n", object->meshName);
  printf ("scene_add_object: mesh without extension = %s\n",
          meshNameWithoutExtension);

  object->mesh = NULL;

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
          object->mesh = scene->meshes[i];

          printf ("scene_add_object: mesh %s assigned to object %s\n",
                  scene->meshes[i]->name, object->name);

          break;
        }
    }

  if (object->mesh == NULL)
    {
      printf ("scene_add_object: kein passendes Mesh gefunden für object %s. "
              "Gesucht: %s\n",
              object->name, meshNameWithoutExtension);
      return;
    }

  scene->objects[scene->object_count] = object;
  scene->object_count++;
}

void
scene_update (Scene *scene)
{
  assert (scene != NULL);

  glBindFramebuffer (GL_FRAMEBUFFER, scene->framebuffer);
  glEnable (GL_DEPTH_TEST);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Object *object = NULL;

  for (int j = 0; j < scene->object_count; j++)
    {
      object = scene->objects[j];
      if (object == NULL)
        {
          continue;
        }

      if (object->mesh == NULL)
        {
          printf ("mesh from object %d in scene is NULL!!!\n", j);
          continue;
        }

      identity (object->modelMatrix);

      object_transformation (object, object->transformation->translation,
                             object->transformation->scaling,
                             object->transformation->rotation);
      object_draw (object, scene->camera->viewProj);
    }

  glBindFramebuffer (GL_FRAMEBUFFER, 0);
  glDisable (GL_DEPTH_TEST);
}
