#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/scene/loadObjectList.h"

/**
 * Lädt aus übergebenen Datei die Objekte und fügt diese in eine Scene hinzu.
 */
int
load_object_list (const char *filename, Scene *scene)
{

  // 1. Objektliste laden:
  FILE *fp = fopen (filename, "r");
  if (fp == NULL)
    {
      printf ("(load_object_list) Datei %s konnte nicht geöffnet werden.\n",
              filename);
      return -1;
    }

  int count = 0;
  char buffer[512];

  // 2. Objektnamen einlesen und zur Scene hinzufügen:
  while (fgets (buffer, sizeof (buffer), fp) && count < MAX_OBJECTS)
    {
      buffer[strcspn(buffer, "\n")] = '\0';
      scene_add_object(scene, buffer);
      ++count;
    }

  fclose (fp);

  return count;
}