#include "../../headers/renderer/mesh.h"
#include "../../headers/scene/scene.h"

void
render_scene (Scene *scene)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(postprocessProgram);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, scene->texturebuffer);

  GLint loc =
      glGetUniformLocation(postprocessProgram,
                           "screenTexture");

  glUniform1i(loc, 0);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
