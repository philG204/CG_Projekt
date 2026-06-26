#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <GL/glew.h>

#include "../../headers/renderer/loadShader.h"
#include "../../headers/renderer/shader.h"

typedef struct ShaderPaths
{
  char vertexPath[512];
  char fragmentPath[512];
} ShaderPaths;

/**
 * @brief Find and extract vertex and fragment shader paths from a directory
 *
 * Searches a directory for shader files containing "vertex" or "fragment"
 * in their names and extracts the full file paths.
 *
 * @param shaderDir Directory path to search for shader files
 * @param outPaths Pointer to ShaderPaths structure to store found paths
 * @return 1 if both shaders found, 0 otherwise
 */
static int
get_shader_paths_from_dir (const char *shaderDir, ShaderPaths *outPaths)
{
  outPaths->vertexPath[0] = '\0';
  outPaths->fragmentPath[0] = '\0';

  DIR *dir = opendir (shaderDir);

  if (dir == NULL)
    {
      printf ("Shader-Ordner konnte nicht geöffnet werden: %s\n", shaderDir);
      return 0;
    }

  struct dirent *entry;

  while ((entry = readdir (dir)) != NULL)
    {
      if (strcmp (entry->d_name, ".") == 0
          || strcmp (entry->d_name, "..") == 0)
        {
          continue;
        }

      char completePath[512];

      snprintf (completePath, sizeof (completePath), "%s/%s", shaderDir,
                entry->d_name);

      struct stat fileStat;

      if (stat (completePath, &fileStat) != 0)
        {
          printf ("Shader-Datei konnte nicht geprüft werden: %s\n",
                  completePath);
          continue;
        }

      if (!S_ISREG (fileStat.st_mode))
        {
          continue;
        }

      if (strstr (entry->d_name, "vertex") != NULL
          || strstr (entry->d_name, "Vertex") != NULL)
        {
          strncpy (outPaths->vertexPath, completePath,
                   sizeof (outPaths->vertexPath));

          outPaths->vertexPath[sizeof (outPaths->vertexPath) - 1] = '\0';
        }
      else if (strstr (entry->d_name, "fragment") != NULL
               || strstr (entry->d_name, "Fragment") != NULL)
        {
          strncpy (outPaths->fragmentPath, completePath,
                   sizeof (outPaths->fragmentPath));

          outPaths->fragmentPath[sizeof (outPaths->fragmentPath) - 1] = '\0';
        }
    }

  closedir (dir);

  if (outPaths->vertexPath[0] == '\0')
    {
      printf ("Kein Vertex-Shader gefunden in: %s\n", shaderDir);
      return 0;
    }

  if (outPaths->fragmentPath[0] == '\0')
    {
      printf ("Kein Fragment-Shader gefunden in: %s\n", shaderDir);
      return 0;
    }

  printf ("Vertex Shader gefunden:   %s\n", outPaths->vertexPath);
  printf ("Fragment Shader gefunden: %s\n", outPaths->fragmentPath);

  return 1;
}

GLuint
shader_init (const char *shaderDir)
{
  assert (shaderDir != NULL);
  GLint status;
  ShaderPaths shaderPath;

  get_shader_paths_from_dir (shaderDir, &shaderPath);

  const char *vertexShaderText = loadShader (shaderPath.vertexPath);
  GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vertexShader, 1, &vertexShaderText, NULL);
  glCompileShader (vertexShader);

  glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &status);

  if (!status)
    {
      printf ("Error compiling vertex shader: ");
      GLchar infoLog[1024];
      glGetShaderInfoLog (vertexShader, 1024, NULL, infoLog);
      printf ("%s\n", infoLog);
    }
  printf ("loaded vertex shader\n");

  const char *fragmentShaderText = loadShader (shaderPath.fragmentPath);

  const GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
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

  const GLuint shaderProgramId = glCreateProgram ();
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
use_shader (const GLuint shaderProgram)
{
  glUseProgram (shaderProgram);
}
