#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../headers/core/window.h"
#include "../../headers/renderer/loadShader.h"

/**
 *  @brief
 *
 *  @param window pointer to the window
 *  @param width
 *  @param height
*/
static void
framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  assert (window != NULL);

  (void)window;
  glViewport (0, 0, width, height);
}

/**
 *  @brief
 *
 *  @param width
 *  @param height
 *  @param title pointer to
 *  @return
*/
Window *
window_create (int width, int height, const char *title)
{
  assert (title != NULL);

  glfwInitHint (GLFW_PLATFORM, GLFW_PLATFORM_X11);
  glfwInit ();
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  Window *window = malloc (sizeof (Window));

  window->window = glfwCreateWindow (width, height, title, NULL, NULL);

  if (!window->window)
    {
      printf ("Failed to create window\n");
      glfwTerminate ();
      return NULL;
    }

  glfwSetFramebufferSizeCallback (window->window, framebuffer_size_callback);
  glfwMakeContextCurrent (window->window);

  GLenum err = glewInit ();
  if (err != GLEW_OK)
    {
      printf ("GLEW error: %s\n", glewGetErrorString (err));
      return 0;
    }

  printf ("OpenGL Version: %s\n", glGetString (GL_VERSION));

  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LESS);

  window->screen_shader = glCreateProgram ();

  GLuint vertex_shader = glCreateShader (GL_VERTEX_SHADER);
  const GLchar *vertex_shader_src = loadShader ("assets/vertex.glsl");

  glShaderSource (vertex_shader, 1, &vertex_shader_src, NULL);
  glCompileShader (vertex_shader);

  GLint success;
  glGetShaderiv (vertex_shader, GL_COMPILE_STATUS, &success);

  if (!success)
    {
      char log[512];
      glGetShaderInfoLog (vertex_shader, sizeof (log), NULL, log);
      printf ("%s\n", log);
    }

  glAttachShader (window->screen_shader, vertex_shader);

  GLuint fragment_shader = glCreateShader (GL_FRAGMENT_SHADER);
  const GLchar *fragment_shader_src = loadShader ("assets/fragment.glsl");

  glShaderSource (fragment_shader, 1, &fragment_shader_src, NULL);
  glCompileShader (fragment_shader);

  glGetShaderiv (fragment_shader, GL_COMPILE_STATUS, &success);

  if (!success)
    {
      char log[512];
      glGetShaderInfoLog (fragment_shader, sizeof (log), NULL, log);
      printf ("%s\n", log);
    }

  glAttachShader (window->screen_shader, fragment_shader);

  glLinkProgram (window->screen_shader);
  glGetProgramiv (window->screen_shader, GL_LINK_STATUS, &success);

  if (!success)
    {
      char log[512];
      glGetProgramInfoLog (window->screen_shader, sizeof (log), NULL, log);
      printf ("%s\n", log);
    }

  glDeleteShader (vertex_shader);
  glDeleteShader (fragment_shader);

  float quad_vertices[]
      = { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
          1.0f,  1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
          1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f };

  GLuint quad_vbo;

  glGenVertexArrays (1, &window->vertex_array);
  glGenBuffers (1, &quad_vbo);

  glBindVertexArray (window->vertex_array);

  glBindBuffer (GL_ARRAY_BUFFER, quad_vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (quad_vertices), quad_vertices,
                GL_STATIC_DRAW);

  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float),
                         (void *)0);
  glEnableVertexAttribArray (0);

  glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof (float),
                         (void *)(2 * sizeof (float)));
  glEnableVertexAttribArray (1);

  glBindVertexArray (0);

  return window;
}
