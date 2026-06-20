#ifndef LOADOBJECTLIST_H
#define LOADOBJECTLIST_H

#include "../../headers/scene/scene.h"
#define OBJECTNAME_LENGTH 256

// Pfad zu den Objektdateien
# define PATH "assets/"



/**
 * Lädt aus übergebenen Datei die Objekte und fügt diese in eine Scene hinzu.
 */
int load_object_list (const char *filename, Scene *scene);
#endif // LOADOBJECTLIST_H
