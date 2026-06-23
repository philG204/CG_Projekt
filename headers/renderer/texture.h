#ifndef TEXTURE_H
#define TEXTURE_H
#define MAX_TEXTURES 8

#include <GL/glew.h>

typedef struct Texture{
    GLuint textureId;
    GLuint shaderProgramId;
    char shaderVariable[256];
}Texture;

Texture* texture_init(char* filename, GLuint shaderProgram, char* shaderVariable);
void activate_texture(Texture* textures[], int texture_count);
int texture_init_from_config(const char *configPath, GLuint shader, Texture **textures, int maxTextures);
#endif // TEXTURE_H
