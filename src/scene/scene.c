#include <GL/glew.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/scene/scene.h"
#include "../../headers/scene/loadObjectList.h"

static void 
getNameWithoutExtension(const char *path, char *out, size_t outSize)
{
    // letzten Slash suchen
    const char *filename = strrchr(path, '/');

    if (filename)
        filename++;   // Slash überspringen
    else
        filename = path;

    // letzte Dateiendung suchen
    const char *dot = strrchr(filename, '.');

    size_t len;
    if (dot)
        len = dot - filename;
    else
        len = strlen(filename);

    if (len >= outSize)
        len = outSize - 1;

    strncpy(out, filename, len);
    out[len] = '\0';
}

Scene *
scene_init (char *meshDir, int mesh_count, char *scene_name,
            CameraSettings *cameraSettings,
            ProjectionSettings *projectionSettings)
{
  Scene *scene = malloc (sizeof (Scene));
  Camera *camera;
  LightDirection light;
  Mesh **meshes = malloc(sizeof(Mesh *) * MAX_MESHES);
  Object **objects = malloc(sizeof(Object*) * MAX_OBJECTS);
  scene->mesh_count = 0;
  scene->object_count = 0;
  scene->objects = objects;
  scene->camera = malloc(sizeof (Camera));

  light.x = 0.0f;
  light.y = 4.0f;
  light.z = 0.0f;
  scene->light = light;

  camera = camera_init (cameraSettings, projectionSettings);
  scene->camera = camera;

  char meshPath[512];
  snprintf (meshPath, sizeof (meshPath), "assets/%s", meshDir);
  printf ("%s\n", meshPath);
  DIR *dir = opendir (meshPath);

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
      char completeMeshPath[512];
      snprintf (completeMeshPath, sizeof (completeMeshPath), "assets/%s/%s",
                meshDir, entry->d_name);
      printf ("%s\n", completeMeshPath);

      printf ("before mesh_init\n");
      meshes[scene->mesh_count] = mesh_init (completeMeshPath);
      printf ("after mesh_init\n");
      scene->mesh_count++;
    }

  scene->meshes = meshes;
  closedir (dir);
  return scene;
}

void
scene_add_object (Scene *scene, char* objDir, char *mesh, float *materialLight,
                  int transparancy)
{
  Object *object = malloc (sizeof (Object));

  object = object_init (objDir, materialLight, transparancy);
  
  char meshName[256];
  getNameWithoutExtension(mesh, meshName, sizeof(meshName));
  strncpy(object->name, objDir, sizeof(object->name) - 1);
  object->name[sizeof(object->name) - 1] = '\0';
  printf("object name: %s\n", object->name);

  for (int i = 0; i < scene->mesh_count; i++)
    {
      if(strcmp(scene->meshes[i]->name, meshName) != 0){
        printf("strcmp: %s, %s\n", scene->meshes[i]->name, meshName);
        perror("strcmp");
      }
      if (strcmp(scene->meshes[i]->name, meshName) == 0)
        {
          printf("setting mesh to object\n");
          object->mesh = scene->meshes[i];
        }
    }
  scene->objects[scene->object_count] = object;
  scene->object_count++;
}

void
scene_update (Scene* scene, sceneObject* objectList, int objectCount, float input)
{
  /*
  for(int i = 0;i < scene->object_count; i++){
    for(int j = 0;j < objectCount; j++){
      printf("Object names are matching object in scene: %s, object in sceneList: %s \n", scene->objects[i]->name, objectList[j].objectName);
        
      if(scene->objects[i]->name == objectList[j].objectName){
        //object_transformation(scene->objects[i], objectList->translation, objectList->scaling, objectList->rotation);
        //object_draw(scene->objects[i], scene->camera->viewProjMatrix);
      }
    }
  }
  */
  for(int j = 0;j < scene->object_count; j++){
    if(scene->objects[j] == NULL){
      printf("object %d in scene ist NULL!!!\n", j);
    }
    if(scene->objects[j]->mesh == NULL){
      printf("mesh from object %d in scene is NULL!!!\n", j);
    }
    object_draw(scene->objects[j], scene->camera->viewProjMatrix);
  }
}
