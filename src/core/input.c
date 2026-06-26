#include <assert.h>

#include <GLFW/glfw3.h>

#include "../../headers/core/input.h"

int keyboardpressed_left = 0;
int keyboardpressed_right = 0;
int keyboardpressed_space = 0;

void
processKeyInput (GLFWwindow *window, const int cameraCount, int *activeCamera,
                 int *processingEnabled)
{
  assert (window != NULL);
  assert (activeCamera != NULL);

  const int left = (glfwGetKey (window, GLFW_KEY_LEFT) == GLFW_PRESS);
  const int right = (glfwGetKey (window, GLFW_KEY_RIGHT) == GLFW_PRESS);
  const int space = (glfwGetKey (window, GLFW_KEY_SPACE) == GLFW_PRESS);

  if (left && !keyboardpressed_left)
    *activeCamera = (*activeCamera - 1 + cameraCount) % cameraCount;

  if (right && !keyboardpressed_right)
    *activeCamera = (*activeCamera + 1) % cameraCount;

  if (space && !keyboardpressed_space)
    *processingEnabled = (*processingEnabled + 1) % 2;

  keyboardpressed_left = left;
  keyboardpressed_right = right;
  keyboardpressed_space = space;
}

/**
 * @brief Checks if mouse is over the Button
 *
 * @param window GLFW window handle
 * @param button button
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

void
handleButtons (GLFWwindow *window, const Button button, const int back,
               const int cameraCount, int *activeCamera)
{
  assert (window != NULL);
  assert (activeCamera != NULL);

  const int hovering = isMouseOverButton (window, button);

  const int state = glfwGetMouseButton (window, GLFW_MOUSE_BUTTON_LEFT);

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
