#include <assert.h>

#include <GLFW/glfw3.h>

#include "../../headers/core/input.h"

/** @brief State tracking for left arrow key press */
int keyboardpressed_left = 0;

/** @brief State tracking for right arrow key press */
int keyboardpressed_right = 0;

/** @brief State tracking for space key press */
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
 * @brief Check if the mouse cursor is over a button
 *
 * Compares the current mouse position with the button's bounding box
 * to determine if the cursor is hovering over it.
 *
 * @param window Pointer to GLFW window handle
 * @param button Button structure with position and dimensions
 * @return 1 if mouse is over button, 0 otherwise
 */
static int
isMouseOverButton (GLFWwindow *window, const Button button)
{
  assert (window != NULL);

  double mouseX, mouseY;
  glfwGetCursorPos (window, &mouseX, &mouseY);

  return mouseX >= button.x && mouseX <= button.x + button.width
         && mouseY >= button.y && mouseY <= button.y + button.height;
}

/** @brief State tracking for left mouse button press to avoid repeated
 * triggers */
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
