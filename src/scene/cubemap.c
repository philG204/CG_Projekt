#include <GL/glew.h>

#include "../../headers/stb_image.h"

#include "../../headers/scene/cubemap.h"


GLuint *cubemap_init (Scene *scene, char *textureFileNames[], char *objectName){

    // 1. Objekt und Texture-Files laden:
    //scene_add_object(scene, objectName);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    GLuint texture = textureID;

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    // 2. Texturen für jede Seite laden und binden:
    int width, height, nrChannels, format;

    for (int i = 0; i < 6; i++) {

        unsigned char *textureData = stbi_load(textureFileNames[i], &width, &height, &nrChannels, 0);

        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
        stbi_image_free(textureData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLuint *pTexture = malloc(sizeof(GLuint));
    *pTexture = texture;
    return pTexture;
}


void cubemap_draw (Object *object, GLfloat *cameraMatrix, GLuint *texture)
{
    const int cubemapLoc = glGetUniformLocation(object->material->shader, "cubemap");
    glUniform1i(cubemapLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

  object_draw(object, cameraMatrix);
}