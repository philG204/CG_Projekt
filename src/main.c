#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../headers/stb_image.h"

#include "../headers/core/input.h"
#include "../headers/core/window.h"
#include "../headers/renderer/postProcessing.h"
#include "../headers/scene/camera.h"
#include "../headers/scene/loadObjectList.h"
#include "../headers/scene/object.h"
#include "../headers/scene/scene.h"

int
main (void)
{

  Window *window = window_create (1920, 1080, "CG1");

  CameraSettings cameraSettings[] = { { .eye = { 3.0f, 2.0f, 5.0f },
                                        .center = { 0.0f, 0.0f, 0.0f },
                                        .up = { 0.0f, 1.0f, 0.0f } },
                                      { .eye = { 3.0f, 4.0f, 5.0f },
                                        .center = { 0.0f, 0.0f, 0.0f },
                                        .up = { 0.0f, 1.0f, 0.0f } },
                                      { .eye = { -6.0f, 4.0f, 5.0f },
                                        .center = { 0.0f, 0.0f, 0.0f },
                                        .up = { 0.0f, 1.0f, 0.0f } } };

  int activeCamera = 0;
  int cameraCount = sizeof (cameraSettings) / sizeof (CameraSettings);
  int lastactivecamera = activeCamera;

  ProjectionSettings projectionSettings = { .fovy = 100.0f * (3.14f / 270.0f),
                                            .aspect = 1920.0f / 1080.0f,
                                            .near_plane = 0.1f,
                                            .far_plane = 45.0f };

  Scene *scene
      = scene_init ("Meshes", "Shaders", "scene1",
                    &cameraSettings[activeCamera], &projectionSettings);

  load_object_list ("room.txt", scene);

  while (!glfwWindowShouldClose (window->window))
    {
      // get input
      processKeyInput (window->window, cameraCount, &activeCamera,
                       &window->processingEnabled);

      // than draw with input;
      // Update Camera when input has change the camera
      // wir können uns natürlich überlgen ob das sinn macht weil die
      // camera_setCameraSettings function überprüft ob die aktive camera
      // settings und die neuen camera settings gleich sind passiert nichts
      if (activeCamera != lastactivecamera)
        {
          camera_setCameraSettings (scene->camera,
                                    &cameraSettings[activeCamera]);
          lastactivecamera = activeCamera;
        }

      camera_update (scene->camera);

      scene_update (scene);

      render_scene (scene, window);

      glfwSwapBuffers (window->window);
      glfwPollEvents ();
    }

  glfwTerminate ();
  return 0;
}
