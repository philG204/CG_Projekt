#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../headers/core/window.h"
#include "../../headers/renderer/mesh.h"
#include "../../headers/scene/scene.h"

void
render_scene (Scene *scene, Window *window)
{
  GLfloat time = glfwGetTime ();

  glClear (GL_COLOR_BUFFER_BIT);

  if (window->processingEnabled)
    {
      glUseProgram (window->processing_shader);
      glUniform1f (glGetUniformLocation (window->processing_shader, "time"),
                   time);
    }
  else
    glUseProgram (window->screen_shader);

  glActiveTexture (GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, scene->texturebuffer);

  glUniform1f (glGetUniformLocation (window->screen_shader, "time"), time);

  glBindVertexArray (window->vertex_array);
  glDrawArrays (GL_TRIANGLES, 0, 6);
}
