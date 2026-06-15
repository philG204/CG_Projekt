#include <GL/glew.h>
#include <stdio.h>

#include "../../headers/renderer/shader.h"
#include "../../headers/scene/light.h"


// Setzt die Beleuchtung nach dem Gouraud-Shading-Verfahren um.
void gouraudLightning(const GLfloat *lightDir, const GLfloat *modelViewMatrix){
    
    if(lightDir == NULL){
        printf("(gouraudLightning) Fehler: LightDirection fehlt!\n");
        return;
    }


    GLuint shaderProg = shader_init("v_lightning.glsl");

    int 
        modelViewMatrixLoc = glGetUniformLocation(shaderProg, "modelViewMatrix"),
        lightPosLoc = glGetUniformLocation(shaderProg, "lightPos");


    glUniformMatrix4fv(modelViewMatrixLoc, 1, GL_FALSE, (GLfloat*)modelViewMatrix);
    glUniform3f(lightPosLoc, lightDir[0], lightDir[1], lightDir[2]);

    
    use_shader(shaderProg);
}