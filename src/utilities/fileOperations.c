#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/utilities/fileOperations.h"

void
getNameWithoutExtension (const char *path, char *out, const size_t outSize)
{
  assert (path != NULL);
  assert (out != NULL);

  // letzten Slash suchen
  const char *filename = strrchr (path, '/');

  if (filename)
    filename++; // Slash überspringen
  else
    filename = path;

  // letzte Dateiendung suchen
  const char *dot = strrchr (filename, '.');

  size_t len;
  if (dot)
    len = dot - filename;
  else
    len = strlen (filename);

  if (len >= outSize)
    len = outSize - 1;

  strncpy (out, filename, len);
  out[len] = '\0';
}
