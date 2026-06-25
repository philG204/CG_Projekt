#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char *
loadShader (const char *filename)
{
  assert (filename != NULL);

  printf ("load shader: %s\n", filename);
  FILE *f = fopen (filename, "rb");
  if (!f)
    {
      perror ("fopen");
      return NULL;
    }

  // Dateigröße bestimmen
  if (fseek (f, 0, SEEK_END) != 0)
    {
      perror ("fseek");
      fclose (f);
      return NULL;
    }

  long length = ftell (f);
  if (length == -1L)
    {
      perror ("ftell");
      fclose (f);
      return NULL;
    }

  rewind (f);

  // Speicher reservieren (+1 für '\0')
  char *buffer = malloc (length + 1);
  if (!buffer)
    {
      perror ("malloc");
      fclose (f);
      return NULL;
    }

  // Datei einlesen
  size_t read_bytes = fread (buffer, 1, length, f);
  if (read_bytes != (size_t)length)
    {
      perror ("fread");
      free (buffer);
      fclose (f);
      return NULL;
    }

  // Null-Terminierung (WICHTIG für OpenGL!)
  buffer[length] = '\0';

  fclose (f);
  return buffer;
}
