#include <assert.h>
#include <stdio.h>

#include <GL/glew.h>

#include "../../headers/renderer/loadShader.h"
#include "../../headers/renderer/shader.h"

GLuint
shader_init (char *objDir)
{
  assert (objDir != NULL);

  printf ("entering shader_init\n");
  GLuint shaderProgramId;

  char vertexShaderPath[512] = { 0 };
  snprintf (vertexShaderPath, sizeof (vertexShaderPath),
            "assets/%s/Shaders/vertexShader.vs", objDir);
  const char *vertexShaderText = loadShader (vertexShaderPath);
  GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vertexShader, 1, &vertexShaderText, NULL);
  glCompileShader (vertexShader);

  GLint status;
  glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &status);

  if (!status)
    {
      printf ("Error compiling vertex shader: ");
      GLchar infoLog[1024];
      glGetShaderInfoLog (vertexShader, 1024, NULL, infoLog);
      printf ("%s\n", infoLog);
    }
  printf ("loaded vertex shader\n");
  char fragmentShaderPath[512] = { 0 };
  snprintf (fragmentShaderPath, sizeof (fragmentShaderPath),
            "assets/%s/Shaders/fragmentShader.vs", objDir);
  const char *fragmentShaderText = loadShader (fragmentShaderPath);

  GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (fragmentShader, 1, &fragmentShaderText, NULL);
  glCompileShader (fragmentShader);

  glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &status);

  if (!status)
    {
      printf ("Error compiling fragment shader: ");
      GLchar infoLog[1024];
      glGetShaderInfoLog (fragmentShader, 1024, NULL, infoLog);
      printf ("%s\n", infoLog);
    }

  shaderProgramId = glCreateProgram ();
  glAttachShader (shaderProgramId, vertexShader);
  glAttachShader (shaderProgramId, fragmentShader);
  glLinkProgram (shaderProgramId);

  glGetProgramiv (shaderProgramId, GL_LINK_STATUS, &status);

  if (!status)
    {
      printf ("Error linking program: ");
      GLchar infoLog[1024];
      glGetProgramInfoLog (shaderProgramId, 1024, NULL, infoLog);
      printf ("%s\n", infoLog);
    }
  glValidateProgram (shaderProgramId);
  glGetProgramiv (shaderProgramId, GL_VALIDATE_STATUS, &status);

  if (!status)
    {
      printf ("Error validating program: ");
      GLchar infoLog[1024];
      glGetProgramInfoLog (shaderProgramId, 1024, NULL, infoLog);
      printf ("%s\n", infoLog);
    }
  glValidateProgram (shaderProgramId);
  glGetProgramiv (shaderProgramId, GL_VALIDATE_STATUS, &status);

  return shaderProgramId;
}

void
use_shader (GLuint shaderProgram)
{
  glUseProgram (shaderProgram);
}
