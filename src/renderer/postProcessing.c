#include "../../headers/core/window.h"
#include "../../headers/renderer/mesh.h"
#include "../../headers/scene/scene.h"

/**
  @brief

  @param scene
  @param window
*/
void
render_scene (Scene *scene, Window *window)
{
  glClear (GL_COLOR_BUFFER_BIT);

  glUseProgram (window->screen_shader);

  glActiveTexture (GL_TEXTURE0);
  glBindTexture (GL_TEXTURE_2D, scene->texturebuffer);

  glBindVertexArray (window->vertex_array);
  glDrawArrays (GL_TRIANGLES, 0, 6);
}
