#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "../../headers/scene/scene.h"


Scene* scene_init(char* meshDir, int mesh_count, char* scene_name, CameraSettings* cameraSettings, ProjectionSettings* projectionSettings){
    Scene* scene = malloc(sizeof(Scene));
    Camera* camera = malloc(sizeof(Camera));
    LightDirection light;
    Mesh* meshes[MAX_MESHES];
    Object* objects[MAX_OBJECTS];
    scene->mesh_count = 0;
    scene->objects = objects;
    
    light.x = 0.0f;
    light.y = 4.0f;
    light.z = 0.0f;
    scene->light = light;

    camera = camera_init(cameraSettings, projectionSettings);
    scene->camera = camera;

    const char meshPath[512];
    snprintf(meshPath, sizeof(meshPath), "assets/%s", meshDir);
    printf("%s\n", meshPath);
    DIR* dir = opendir(meshPath);
    
    if(dir == NULL){
        printf("Ordner konnte nicht geöffnet werden.\n");
        return NULL;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;    
        }
        const char completeMeshPath[512];
        snprintf(completeMeshPath, sizeof(completeMeshPath), "assets/%s/%s", meshDir, entry->d_name);
        printf("%s\n", completeMeshPath);
  
        printf("before mesh_init\n");
        meshes[scene->mesh_count] = mesh_init(completeMeshPath);
        printf("after mesh_init\n");
        scene->mesh_count++;
    }
   
    scene->meshes = meshes;
    closedir(dir);
    return scene;
}

void scene_add_object(Scene* scene, char* objDir, char* mesh, float* materialLight, int transparancy){
    printf("entering scene_add_object\n");
    Object* object = malloc(sizeof(Object));

    object = object_init(objDir, materialLight, transparancy);
    object->name = objDir;

    for(int i=0;i<scene->mesh_count;i++){
        if(scene->meshes[i] == mesh){
            object->mesh = scene->meshes[i];
        }
    }
    scene->objects[scene->object_count] = object;
    scene->object_count++;
}

void scene_update(Scene scene, float input){

}