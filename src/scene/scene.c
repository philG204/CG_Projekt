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
  printf("entering scene_init\n");
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
scene_add_object(Scene *scene, char* objDir)
{
  printf("entering scene_add_object\n");
  Object *object = object_init (objDir);

    if (scene == NULL) {
        printf("scene_add_object: scene is NULL\n");
        return;
    }

    if (object == NULL) {
        printf("scene_add_object: object is NULL\n");
        return;
    }

    if (scene->object_count >= MAX_OBJECTS) {
        printf("scene_add_object: MAX_OBJECTS erreicht\n");
        return;
    }

    if (object->meshName[0] == '\0') {
        printf("scene_add_object: object %s hat keinen meshName aus der Config\n",
               object->name);
        return;
    }

    char meshNameWithoutExtension[256];

    getNameWithoutExtension(object->meshName,
                            meshNameWithoutExtension,
                            sizeof(meshNameWithoutExtension));

    printf("scene_add_object: object = %s\n", object->name);
    printf("scene_add_object: mesh from config = %s\n", object->meshName);
    printf("scene_add_object: mesh without extension = %s\n",
           meshNameWithoutExtension);

    object->mesh = NULL;

    for (int i = 0; i < scene->mesh_count; i++) {
        if (scene->meshes[i] == NULL) {
            continue;
        }

        printf("scene_add_object: compare '%s' with '%s'\n",
               scene->meshes[i]->name,
               meshNameWithoutExtension);

        if (strcmp(scene->meshes[i]->name, meshNameWithoutExtension) == 0) {
            object->mesh = scene->meshes[i];

            printf("scene_add_object: mesh %s assigned to object %s\n",
                   scene->meshes[i]->name,
                   object->name);

            break;
        }
    }

    if (object->mesh == NULL) {
        printf("scene_add_object: kein passendes Mesh gefunden für object %s. Gesucht: %s\n",
               object->name,
               meshNameWithoutExtension);
        return;
    }

    scene->objects[scene->object_count] = object;
    scene->object_count++;
}

void
scene_update(Scene* scene, sceneObject* objectList, int objectCount, float input)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Test Daten
    static GLfloat boxAngle[3] = {0.0f, 0.0f, 0.0f};
    static GLfloat teapotAngle[3] = {0.0f, 0.0f, 0.0f};

    //boxAngle[0] += 0.02f;
    //boxAngle[1] += 0.02f;
    //boxAngle[2] += 0.02f;

    teapotAngle[0] += 0.001f;
    teapotAngle[1] += 0.002f;
    teapotAngle[2] += 0.003f;

    for (int j = 0; j < scene->object_count; j++) {
        if (scene->objects[j] == NULL) {
            continue;
        }

        if (scene->objects[j]->mesh == NULL) {
            printf("mesh from object %d in scene is NULL!!!\n", j);
            continue;
        }

        
        identity(scene->objects[j]->modelMatrix);
        /*
        if (strcmp(scene->objects[j]->name, "Box1") == 0) {
            //GLfloat translateOffset[3] = {-3.5f, 0.0f, 2.0f};
            object_transformation(scene->objects[j], translateOffset, NULL, boxAngle);
        }

        if (strcmp(scene->objects[j]->name, "Box2") == 0) {
            GLfloat translateOffset[3] = {-2.0f, 0.0f, -4.0f};
            object_transformation(scene->objects[j], translateOffset, NULL, boxAngle);
        }


        if (strcmp(scene->objects[j]->name, "Box3") == 0) {
            GLfloat translateOffset[3] = {0.0f, 0.0f, -1.0f};
            object_transformation(scene->objects[j], translateOffset, NULL, boxAngle);
        }

        if (strcmp(scene->objects[j]->name, "Teapot") == 0) {
            GLfloat translateOffset[3] = {2.0f, 0.0f, 0.0f};
            object_transformation(scene->objects[j], translateOffset, NULL, teapotAngle);
        }
        */
        object_transformation(scene->objects[j], scene->objects[j]->transformation->translation, scene->objects[j]->transformation->scaling, scene->objects[j]->transformation->rotation);
        object_draw(scene->objects[j], scene->camera->viewProjMatrix);
    }
}

