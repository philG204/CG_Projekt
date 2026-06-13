
#ifndef LOADOBJECTLIST_H
#define LOADOBJECTLIST_H


// Objekt mit Objektdateinamen  und Positionsparametern in der Szene
typedef struct sceneObject {
    char filename[256];             
    GLfloat translation[4];   
    GLfloat rotation[3];        
    GLfloat scaling;          
} sceneObject;

/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in eine objectList mit zugehöriger Transpositions- Rotations- und Scalingvektoren.
 */
sceneObject *load_object_list(const char * fileName, int *objectCount, int maxObjects);

#endif