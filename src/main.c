#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"

#include "../headers/core/input.h"
#include "../headers/core/window.h"
#include "../headers/scene/camera.h"
#include "../headers/scene/object.h"
#include "../headers/scene/scene.h"
#include "../headers/scene/loadObjectList.h"

int
main (void)
{

  GLFWwindow *window = window_create (1920, 1080, "CG1");

  CameraSettings cameraSettings = { .eye = { 3.0f, 2.0f, 5.0f },
                                    .center = { 0.0f, 0.0f, 0.0f },
                                    .up = { 0.0f, 1.0f, 0.0f } };
    
  ProjectionSettings projectionSettings = { .fovy = 100.0f * (3.14f / 270.0f),
                                            .aspect = 1920.0f / 1080.0f,
                                            .near_plane = 0.1f,
                                            .far_plane = 45.0f };

  Scene *scene = scene_init ("Meshes", 1, "scene1", &cameraSettings,
                             &projectionSettings);


  int objectCnt= 0;
  sceneObject *objectList = load_object_list("room.txt", &objectCnt, 12);    

  float light[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

  scene_add_object (scene, "Box1");
  scene_add_object (scene, "Box2");
  scene_add_object (scene, "Box3");
  scene_add_object (scene, "Box4");
  scene_add_object (scene, "Box5");
  //scene_add_object (scene, "Teapot");
  //scene_add_object (scene, "Earth");
  
  
  
  while (!glfwWindowShouldClose (window))
    {
      // get input
      // than draw with input;
      glfwSwapBuffers (window);
      glfwPollEvents ();

      scene_update(scene, objectList, objectCnt, 0.0f);
    }

  glfwTerminate ();
  return 0;
}
