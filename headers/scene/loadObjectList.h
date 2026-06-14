
#ifndef LOADOBJECTLIST_H
#define LOADOBJECTLIST_H

#define PATH_LENGTH 256


// Speichert den Pfad zur Objekt-Datei und die Position, Rotation, und Skalierung des Objekts.
typedef struct sceneObject {
    char filename[PATH_LENGTH];             
    GLfloat translation[4];   
    GLfloat rotation[3];        
    GLfloat scaling;          
} sceneObject;

/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in ein sceneObject-Array mit zugehörigen Translations- Rotations- und Scalingwerten.
 */
sceneObject *load_object_list(const char * fileName, int *objectCount, int maxObjects);

#endif