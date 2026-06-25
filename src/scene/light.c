#include <assert.h>

#include <GL/glew.h>

#include "../../headers/renderer/shader.h"
#include "../../headers/scene/light.h"


void
gouraudLightning (const GLfloat *lightDir, const GLfloat *modelViewMatrix)
{
  assert (lightDir != NULL);
  assert (modelViewMatrix != NULL);

  GLuint shaderProg = shader_init ("v_lightning.glsl");

  int modelViewMatrixLoc
      = glGetUniformLocation (shaderProg, "modelViewMatrix"),
      lightPosLoc = glGetUniformLocation (shaderProg, "lightPos");

  glUniformMatrix4fv (modelViewMatrixLoc, 1, GL_FALSE,
                      (GLfloat *)modelViewMatrix);
  glUniform3f (lightPosLoc, lightDir[0], lightDir[1], lightDir[2]);

  use_shader (shaderProg);
}
