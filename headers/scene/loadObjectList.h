#ifndef LOADOBJECTLIST_H
#define LOADOBJECTLIST_H

#define OBJECTNAME_LENGTH 256

// Pfad zu den Objektdateien
# define PATH "assets/test/"


// Speichert den Pfad zur Objekt-Datei und die Position, Rotation, und Skalierung des Objekts.
typedef struct sceneObject {
    char objectName[OBJECTNAME_LENGTH];    
} sceneObject;

/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in ein sceneObject-Array mit zugehörigen Translations- Rotations- und Scalingwerten.
 */
int load_object_list (const char *filename, sceneObject objectList[]);
#endif // LOADOBJECTLIST_H
