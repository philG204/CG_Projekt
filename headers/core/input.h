/**
 * @file input.h
 * @brief Input handling for keyboard and UI button interactions
 *
 * This module handles user input through keyboard and on-screen button clicks,
 * managing camera switching and post-processing effects.
 */

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

/**
 * @struct Button
 * @brief Represents a clickable button on the screen
 *
 * @var Button::x
 *      X coordinate of the button's left edge (normalized -1 to 1)
 * @var Button::y
 *      Y coordinate of the button's bottom edge (normalized -1 to 1)
 * @var Button::width
 *      Width of the button in normalized coordinates
 * @var Button::height
 *      Height of the button in normalized coordinates
 */
typedef struct Button
{
  GLfloat x;
  GLfloat y;
  GLfloat width;
  GLfloat height;
} Button;

/**
 * @brief Processes input from the Keyboards and updates the active camera.
 *
 * @param window            Pointer to GLFW window handle
 * @param cameraCount       number of available cameras
 * @param activeCamera      pointer to current camera index
 * @param processingEnabled wether post-processing should be enabled
 */
void processKeyInput (GLFWwindow *window, int cameraCount, int *activeCamera,
                      int *processingEnabled);

/**
 * @brief Processes input from the buttons on the screen and updates the active
 * camera.
 *
 * @param window        Pointer to GLFW window handle
 * @param button        button
 * @param back          back = 0 before camera | back = 1 after camera
 * @param cameraCount   number of available cameras
 * @param activeCamera  pointer to current camera index
 */
void handleButtons (GLFWwindow *window, Button button, int back,
                    int cameraCount, int *activeCamera);

#endif // INPUT_H
