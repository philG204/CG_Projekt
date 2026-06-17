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
/*
  char texturePath[512];
  snprintf (texturePath, sizeof (texturePath), "assets/%s", textureDir);
  printf ("%s\n", texturePath);
  DIR *dirTexture = opendir (texturePath);

  if (dirTexture == NULL)
    {
      printf ("Ordner konnte nicht geöffnet werden.\n");
      return NULL;
    }

  struct dirent *entryTexture;

  while ((entryTexture = readdir (dir)) != NULL)
    {
      if (strcmp (entryTexture->d_name, ".") == 0
          || strcmp (entryTexture->d_name, "..") == 0)
        {
          continue;
        }
      char completeTexturePath[512];
      snprintf (completeTexturePath, sizeof (completeTexturePath), "assets/%s/%s",
                meshDir, entryTexture->d_name);
      printf ("%s\n", completeTexturePath);

      printf ("before texture_init\n");
      meshes[scene->texture_count] = texture_init (completeTexturePath);
      printf ("after texture_init\n");
      scene->texture_count++;
    }

  scene->textures = textures;
  closedir (dirTexture);
*/
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

/*
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
*/
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

        if (strcmp(scene->objects[j]->name, "Box1") == 0) {
            GLfloat translateOffset[3] = {-3.5f, 0.0f, 2.0f};
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

        object_draw(scene->objects[j], scene->camera->viewProjMatrix);
    }
}

/*
void
scene_update (Scene* scene, sceneObject* objectList, int objectCount, float input)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  for(int i = 0;i < scene->object_count; i++){
    for(int j = 0;j < objectCount; j++){
      printf("Object names are matching object in scene: %s, object in sceneList: %s \n", scene->objects[i]->name, objectList[j].objectName);
        
      if(scene->objects[i]->name == objectList[j].objectName){
        //object_transformation(scene->objects[i], objectList->translation, objectList->scaling, objectList->rotation);
        //object_draw(scene->objects[i], scene->camera->viewProjMatrix);
      }
    }
  }
  
  
  //for(int i=0;i<scene->object_count;i++){
    //object_transformation();
  //}

  for(int j = 0;j < scene->object_count; j++){
    if(strcmp(scene->objects[j]->name, "Box") == 0){
      static GLfloat angleView[3] = {0.02f, 0.05f, 0.03f};
      angleView[0] += 0.02f;
      angleView[1] += 0.02f;
      angleView[2] += 0.02f;
      //float radius = 5.0f;
      //printf("angleView: %f\n", angleView);
      GLfloat translateOffset[3] = {1.0f, 0.0f, 2.0f};
      object_transformation(scene->objects[j], translateOffset, NULL, angleView);
    }
    if(strcmp(scene->objects[j]->name, "Teapot") == 0){
      GLfloat translateOffset[3] = {4.0f, 0.0f, 0.0f};
      static GLfloat angleView[3] = {0.001f, 0.002f, 0.003f};
      angleView[0] += 0.02f;
      angleView[1] += 0.02f;
      angleView[2] += 0.02f;
      //float radius = 5.0f;
      //printf("angleView: %f\n", angleView);

      object_transformation(scene->objects[j], translateOffset, NULL, angleView);
    }
    object_draw(scene->objects[j], scene->camera->viewProjMatrix);
  }
}
*/
