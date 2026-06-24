#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct
{
  GLFWwindow *window;
  GLuint vertex_array;
  GLuint screen_shader;
  GLuint processing_shader;
  int processingEnabled;
} Window;

Window *window_create (int width, int height, const char *title);

#endif // WINDOW_H
