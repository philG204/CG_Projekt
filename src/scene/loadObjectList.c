#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/scene/loadObjectList.h"
# define PATH "assets/models/"


/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in ein sceneObject-Array mit zugehörigen Translations- Rotations- und Scalingvektoren.
 */
sceneObject *load_object_list(const char *filename, int *objectCount, int maxObjects){

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File %s could not be opened.\n", filename);
        return NULL;
    }

    // Speicherplatz für n Objekte mit Pos.werten reservieren.
    sceneObject *sceneObjects = NULL;
    sceneObjects = malloc(sizeof(sceneObject) * maxObjects);

    // Gelesene Objekte
    int count = 0;  

    // Max. 1024 Zeichen pro Zeile einelesen
    char readLine[512];

    while(fgets(readLine, sizeof(readLine), fp) && count < maxObjects){

        sceneObject *obj = &sceneObjects[count];

        int finishedLine = sscanf(
            readLine,

            "%255[^;];%f,%f,%f,%f;%f,%f,%f;%f",

            obj->filename,
            &obj->translation[0],
            &obj->translation[1],
            &obj->translation[2],
            &obj->translation[3],
            &obj->rotation[0],
            &obj->rotation[1],
            &obj->rotation[2],
            &obj->scaling
        );


        if(finishedLine == 9){
            ++count;
        }
    }

    fclose(fp);

    *objectCount = count;

    return sceneObjects;
}