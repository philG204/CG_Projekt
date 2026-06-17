#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/scene/loadObjectList.h"




/**
 * Lädt aus übergebenen Datei die Objekte und speichert diese in ein sceneObject-Array mit zugehörigen Translations- Rotations- und Scalingwerten.
 */
sceneObject *load_object_list(const char *filename, int *objectCount, int maxObjects){

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("(load_object_list) Datei %s konnte nicht geöffnet werden.\n", filename);
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
        char tempFilename[PATH_LENGTH-sizeof(PATH)];
        char objectName[PATH_LENGTH-sizeof(PATH)];

        int finishedLine = sscanf(
            readLine,

            "%255[^;];%f,%f,%f,%f;%f,%f,%f;%f,%f,%f",

            tempFilename,
            &obj->objectName,
            &obj->translation[0],
            &obj->translation[1],
            &obj->translation[2],
            &obj->translation[3],
            &obj->rotation[0],
            &obj->rotation[1],
            &obj->rotation[2],
            &obj->scaling[0],
            &obj->scaling[1],
            &obj->scaling[2]

        );

        strcpy(obj->filename, PATH);
        strcat(obj->filename, tempFilename);
        strcpy(obj->objectName, objectName);


        if(finishedLine == 9){
            ++count;
        }
    }

    fclose(fp);

    *objectCount = count;

    return sceneObjects;
}