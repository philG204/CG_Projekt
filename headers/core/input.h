#ifndef INPUTH
#define INPUTH

#include <GLFW/glfw3.h>

// TODO THIS IS HERE TO EXISTS BECAUSE I NEED IT FOR THE BUTTONS INPUT CHECK
typedef struct Button
{
    float x;
    float y;
    float width;
    float height;
} Button;

/*
 * @brief Processes input from the Keyboards and updates the active camera.
 *
 * @param *window        - GLFW window handle
 * @param cameraCount   - number of available cameras
 * @param *activeCamera  - pointer to current camera index
 */
void processKeyInput (GLFWwindow *window, int cameraCount,  int* activeCamera);

/*
 * @brief Processes input from the buttons on the screen and updates the active camera.
 *
 * @param *window        - GLFW window handle
 * @param button        - button
 * @param back          - back = 0 before camera | back = 1 after camera
 * @param cameraCount   - number of available cameras
 * @param *activeCamera  - pointer to current camera index
 */
void handleButtons(GLFWwindow* window, Button button, int back, int cameraCount,  int *activeCamera);

#endif