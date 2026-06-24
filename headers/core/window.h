#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct
{
  GLFWwindow *window;
  GLuint screen_shader;
  GLuint vertex_array;
} Window;

Window *window_create (int width, int height, const char *title);

#endif // WINDOW_H
