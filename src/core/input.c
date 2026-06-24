#include <assert.h>

#include <GLFW/glfw3.h>

#include "../../headers/core/input.h"

int keyboardpressed_left = 0;
int keyboardpressed_right = 0;
/*
 * @brief Processes input from the Keyboards and updates the active camera.
 *
 * @param *window        - GLFW window handle
 * @param cameraCount   - number of available cameras
 * @param *activeCamera  - pointer to current camera index
 */
void
processKeyInput (GLFWwindow *window, int cameraCount, int *activeCamera)
{
  assert (window != NULL);
  assert (activeCamera != NULL);

  int left = (glfwGetKey (window, GLFW_KEY_LEFT) == GLFW_PRESS);
  int right = (glfwGetKey (window, GLFW_KEY_RIGHT) == GLFW_PRESS);

  if (left && !keyboardpressed_left)
    {
      *activeCamera = (*activeCamera - 1 + cameraCount) % cameraCount;
    }

  if (right && !keyboardpressed_right)
    {
      *activeCamera = (*activeCamera + 1) % cameraCount;
    }

  keyboardpressed_left = left;
  keyboardpressed_right = right;
}

/*
 * @brief Checks if mouse is over the Button
 *
 * @param *window        - GLFW window handle
 * @param button         - button
 */
int
isMouseOverButton (GLFWwindow *window, const Button button)
{
  assert (window != NULL);

  double mouseX, mouseY;
  glfwGetCursorPos (window, &mouseX, &mouseY);

  return mouseX >= button.x && mouseX <= button.x + button.width
         && mouseY >= button.y && mouseY <= button.y + button.height;
}

int leftMouseWasPressed = 0;

/*
 * @brief Processes input from the buttons on the screen and updates the active
 * camera.
 *
 * @param *window        - GLFW window handle
 * @param button         - button
 * @param back           - 0 = advance to next camera | 1 = go to previous
 * camera
 * @param cameraCount    - number of available cameras
 * @param *activeCamera  - pointer to current camera index
 */
void
handleButtons (GLFWwindow *window, Button button, int back, int cameraCount,
               int *activeCamera)
{
  assert (window != NULL);
  assert (activeCamera != NULL);

  int hovering = isMouseOverButton (window, button);

  int state = glfwGetMouseButton (window, GLFW_MOUSE_BUTTON_LEFT);

  if (hovering && state == GLFW_PRESS && !leftMouseWasPressed)
    {
      if (back == 0)
        {
          *activeCamera = (*activeCamera + 1) % cameraCount;
        }
      else
        {
          *activeCamera = (*activeCamera - 1 + cameraCount) % cameraCount;
        }
    }

  leftMouseWasPressed = (state == GLFW_PRESS);
}
