#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/scene/loadObjectList.h"
#include "../../headers/scene/scene.h"


/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in ein
 * sceneObject-Array mit zugehörigen Translations- Rotations- und
 * Scalingwerten.
 */
int
load_object_list (const char *filename, sceneObject objectList[])
{

  FILE *fp = fopen (filename, "r");
  if (fp == NULL)
    {
      printf ("(load_object_list) Datei %s konnte nicht geöffnet werden.\n",
              filename);
      return -1;
    }

  //sceneObject *objectList = NULL;
  //objectList = malloc (sizeof (sceneObject) * maxObjects);

  int count = 0;
  char buffer[512];

  while (fgets (buffer, sizeof (buffer), fp) && count < MAX_OBJECTS)
    {
      buffer[strcspn(buffer, "\n")] = '\0';
      strcpy(objectList[count].objectName, buffer);

      ++count;
    }

  fclose (fp);

  return count;
}