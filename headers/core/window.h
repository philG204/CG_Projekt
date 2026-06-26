/**
 * @file window.h
 * @brief Window creation and management for OpenGL rendering
 *
 * This module handles the creation and initialization of the main window,
 * OpenGL context setup, and post-processing pipeline configuration using
 * GLFW and GLEW.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @struct Window
 * @brief Represents the main rendering window with its OpenGL resources
 *
 * @var Window::window
 *      GLFW window handle
 * @var Window::vertex_array
 *      Vertex Array Object for the post-processing quad
 * @var Window::screen_shader
 *      Shader program for rendering to screen
 * @var Window::processing_shader
 *      Shader program for post-processing effects
 * @var Window::processingEnabled
 *      Flag to enable/disable post-processing (1=enabled, 0=disabled)
 */
typedef struct
{
  GLFWwindow *window;
  GLuint vertex_array;
  GLuint screen_shader;
  GLuint processing_shader;
  int processingEnabled;
} Window;

/**
 * @brief Create the window and set up the post-processing pipeline
 *
 * Initializes GLFW, creates an OpenGL 3.3 core profile window, initializes
 * GLEW, and sets up the post-processing pipeline with screen and processing
 * shaders. Also enables depth testing and alpha blending.
 *
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param title Window title string
 * @return Pointer to initialized Window structure, or NULL if creation failed
 */
Window *window_create (int width, int height, const char *title);

#endif // WINDOW_H
