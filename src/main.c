
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "../headers/core/window.h"
#include "../headers/core/input.h"
#include "../headers/scene/camera.h"
#include "../headers/scene/scene.h"
#include "../headers/scene/object.h"



int main(void){

    
    GLFWwindow* window = window_create(1920, 1080, "CG1");

    CameraSettings cameraSettings = {
        .eye = {1.0f, 0.5f, 1.0f},
        .center = {0.0f, 0.0f, 0.0f},
        .up = {0.0f, 1.0f, 0.0f}
    };

    ProjectionSettings projectionSettings = {
        .fovy = 10.0f * (3.14f / 270.0f),
        .aspect = 1920.0f / 1080.0f,
        .near_plane = 0.1f,
        .far_plane = 45.0f
    };

    Scene* scene = scene_init("Meshes", 1, "scene1", &cameraSettings, &projectionSettings);

    float light[4];
    light[0] = 0.0f;
    light[1] = 0.0f;
    light[2] = 0.0f;
    light[3] = 0.0f;

    scene_add_object(scene, "Box", "cube.obj", light, 1);

    while(!glfwWindowShouldClose(window)){
        //get input
        //than draw with input;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}